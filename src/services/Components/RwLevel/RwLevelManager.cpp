#include "RwLevelManager.h"
#include "RwLevelResourcesDesc.h"

CRwLevelManager::CRwLevelManager()
{
}

CRwLevelManager::~CRwLevelManager()
{
}

void CRwLevelManager::SetRootDir(const std::wstring& dir)
{
	m_RootDir = dir;
}

std::wstring CRwLevelManager::GetRootDir() const
{
	return m_RootDir;
}

bool CRwLevelManager::LoadLevel(const std::string& level_name)
{
	ILevel* level = new CRwLevel();
	level->SetDeserializer(&m_DefaultLvlDeserializer);

	CRwLevelResourcesDesc rsc_desc;
	if (!rsc_desc.InitFromDir(GetRootDir(), level_name))
		return false;

	bool ret = level->Load(&rsc_desc);

	if (ret)
		m_LoadedLevels.push_back(level);

	return ret;
}

bool CRwLevelManager::UnloadLevel(const std::string& level_name)
{
	return false;
}

bool CRwLevelManager::SaveLevel(const std::string& level_name)
{
	return false;
}

bool CRwLevelManager::UnloadAllLevels()
{
	return false;
}

bool CRwLevelManager::SaveAllLevels()
{
	return false;
}