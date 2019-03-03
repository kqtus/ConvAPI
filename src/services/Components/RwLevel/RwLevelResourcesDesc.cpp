#include "RwLevelResourcesDesc.h"
#include <filesystem>
#include <regex>
#include "../../utilities/stream.h"

namespace fs = std::experimental::filesystem;

bool CRwLevelResourcesDesc::InitFromDir(const std::wstring& dir_name, const std::string& level_name)
{
	// 1. Look for gta*.dat and open it.
	// 2. Find all IDEs and IPLs with the name of the level.
	// 3. Additionaly try to find IMG with given level name.
	// 4. Find gta3.img (.dir) file path.
	// 5. Save found paths.

	auto is_gta_dat = [](const wchar_t* dir_path)
	{
		return std::wstring(dir_path).find(L"gta_vc.dat") != std::wstring::npos;
		//return std::regex_match(dir_path, std::wregex(L"^gta_vc.dat"));
	};

	auto is_gta3_img = [](const wchar_t* dir_path)
	{
		return std::wstring(dir_path).find(L"gta3.img") != std::wstring::npos;
		//return std::regex_match(dir_path, std::wregex(L"gta3.img"));
	};

	std::wstring gta_dat_path;
	std::wstring gta3_img_path;

	for (auto& dir : fs::recursive_directory_iterator(dir_name))
	{
		auto path = dir.path();

		if (is_gta_dat(path.c_str()))
			gta_dat_path = path.c_str();
		if (is_gta3_img(path.c_str()))
			gta3_img_path = path.c_str();
	}

	if (gta_dat_path.empty() || gta3_img_path.empty())
		return false;

	m_UsedImgPaths.push_back(gta3_img_path);

	in_stream<EStreamType::TEXT> gta_dat_strm;

	if (!gta_dat_strm.Open(gta_dat_path.c_str()))
		return false;

	auto is_proper_ipl_path = [&](const char* rel_path)
	{
		return std::string(rel_path).find(level_name + ".ipl") != std::string::npos;
		//return std::regex_match(rel_path, std::regex(level_name + ".ipl"));
	};

	auto is_proper_ide_path = [&](const char* rel_path)
	{
		return std::string(rel_path).find(level_name + ".ide") != std::string::npos;
		//return std::regex_match(rel_path, std::regex(level_name + ".img"));
	};

	auto make_path = [&](const std::string& gta_dat_entry) -> std::wstring
	{
		const size_t entry_offset = 4;
		std::wstring w_entry(gta_dat_entry.begin() + 4, gta_dat_entry.end());
		
		return std::wstring(fs::path(dir_name + w_entry).generic_wstring()) + L'\0';
	};

	while (!gta_dat_strm.Eof())
	{
		std::string line;
		READ_LINE(gta_dat_strm, line);
		std::transform(line.begin(), line.end(), line.begin(), ::tolower);

		if (is_proper_ide_path(line.c_str()))
			m_IdePaths.push_back(make_path(line));
		if (is_proper_ipl_path(line.c_str()))
			m_IplPaths.push_back(make_path(line));
	}

	gta_dat_strm.Close();
	return true;
}

CRwLevelResourcesDesc* CRwLevelResourcesDesc::Clone() const
{
	return new CRwLevelResourcesDesc(*this);
}

std::string CRwLevelResourcesDesc::GetName() const
{
	return m_LevelName;
}

void CRwLevelResourcesDesc::GetUsedArchivePaths(TWStrVec& out_paths) const
{
	for (auto& img_path : m_UsedImgPaths)
		out_paths.push_back(img_path);
}

void CRwLevelResourcesDesc::GetItemPlacementFilePaths(TWStrVec& out_paths) const
{
	for (auto& ipl_path : m_IplPaths)
		out_paths.push_back(ipl_path);
}

void CRwLevelResourcesDesc::GetItemDefinitionFilePaths(TWStrVec& out_paths) const
{
	for (auto& ide_path : m_IdePaths)
		out_paths.push_back(ide_path);
}
