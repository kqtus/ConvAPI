#include <algorithm>

template<rw::rs::EArchiveVer arch_ver>
inline bool CRwLevelResourcesDAO::Connect(const std::vector<std::string>& archive_paths)
{
	for (auto& path : archive_paths)
	{
		if (!TryAddArchive<arch_ver>(path))
		{
			Disconnect();
			return false;
		}
	}

	return true;
}

template<rw::rs::EArchiveVer arch_ver>
inline bool CRwLevelResourcesDAO::Connect(const std::vector<std::wstring>& archive_paths)
{
	for (auto& path : archive_paths)
	{
		if (!TryAddArchive<arch_ver>(path))
		{
			Disconnect();
			return false;
		}
	}

	return true;
}

template<rw::rs::EArchiveVer arch_ver, class TPath>
inline bool CRwLevelResourcesDAO::TryAddArchive(const TPath& path)
{
	in_stream<EStreamType::BINARY> arch_strm;

	if (!arch_strm.Open(path.c_str()))
		return false;

	rw::rs::archive<arch_ver> archive;
	archive.Read(arch_strm);
	AddArchive(archive, path);

	return true;
}

template<>
inline void CRwLevelResourcesDAO::AddArchive(const rw::rs::archive<rw::rs::EArchiveVer::VER1>& archive, const std::wstring& path)
{
	m_Ver1Archives.push_back(std::make_pair(archive, path));
}

template<>
inline void CRwLevelResourcesDAO::AddArchive(const rw::rs::archive<rw::rs::EArchiveVer::VER2>& archive, const std::wstring& path)
{
	m_Ver2Archives.push_back(std::make_pair(archive, path));
}

inline bool CRwLevelResourcesDAO::Disconnect(const std::wstring& archive_path)
{
	auto try_erase_in_archive = [&](auto& archive)
	{
		auto itr = std::find_if(archive.begin(), archive.end(), [&](auto& p)
		{
			return p.second == archive_path;
		});

		if (itr == archive.end())
			return false;

		archive.erase(itr);
		return true;
	};

	return try_erase_in_archive(m_Ver1Archives)
		|| try_erase_in_archive(m_Ver2Archives);
}

inline void CRwLevelResourcesDAO::Disconnect()
{
	m_Ver1Archives.clear();
	m_Ver2Archives.clear();
}

inline rw::core::clump* CRwLevelResourcesDAO::GetModel(const std::string& name) const
{
	return GetFile<rw::core::clump>(name);
}

inline rw::core::texture_dictionary* CRwLevelResourcesDAO::GetTexDict(const std::string& name) const
{
	return GetFile<rw::core::texture_dictionary>(name);
}

inline bool CRwLevelResourcesDAO::FindFileStream(const std::string& file_name, in_stream<EStreamType::BINARY>& out_strm) const
{
	for (const auto& arch : m_Ver1Archives)
		if (arch.first.GetFileStream(out_strm, file_name))
			return true;

	for (auto& arch : m_Ver2Archives)
		if (arch.first.GetFileStream(out_strm, file_name))
			return true;

	return false;
}

template<class TRsc>
inline TRsc* CRwLevelResourcesDAO::GetFile(const std::string& name) const
{
	in_stream<EStreamType::BINARY> strm;

	if (FindFileStream(name, strm))
	{
		auto rsc = new TRsc();

		// Do not close this stream here. It is managed by archive.
		rsc->Read(strm);
		return rsc;
	}

	return nullptr;
}