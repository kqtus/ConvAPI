#pragma once
#include <vector>
#include <map>

#include "../Level/ILevelResourcesDAO.h"
#include "../../formats/rw/rwrs.h"
#include "../../formats/rw/rwcore.h"

class CRwLevelResourcesDAO : public ILevelResourcesDAO
{
	template<rw::rs::EArchiveVer arch_ver>
	using TArchives = std::vector<std::pair<rw::rs::archive<arch_ver>, std::wstring>>;
	using TVer1Archives = TArchives<rw::rs::EArchiveVer::VER1>;
	using TVer2Archives = TArchives<rw::rs::EArchiveVer::VER2>;

public:
	template<rw::rs::EArchiveVer arch_ver>
	bool Connect(const std::vector<std::string>& archive_paths);

	template<rw::rs::EArchiveVer arch_ver>
	bool Connect(const std::vector<std::wstring>& archive_paths);

	virtual bool Disconnect(const std::wstring& archive_path) override;
	virtual void Disconnect() override;

	rw::core::clump* GetModel(const std::string& name) const;
	rw::core::texture_dictionary* GetTexDict(const std::string& name) const;

protected:
	template<rw::rs::EArchiveVer arch_ver, class TPath>
	bool TryAddArchive(const TPath& path);

	template<rw::rs::EArchiveVer arch_ver>
	void AddArchive(const rw::rs::archive<arch_ver>& archive, const std::wstring& path);

	bool FindFileStream(const std::string& file_name, in_stream<EStreamType::BINARY>& out_strm) const;

	template<class TRsc>
	TRsc* GetFile(const std::string& name) const;

protected:
	TVer1Archives m_Ver1Archives;
	TVer2Archives m_Ver2Archives;
};

#include "RwLevelResourcesDAO_impl.h"

