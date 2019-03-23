#include "RwLevelManager.h"
#include "RwLevelResourcesDesc.h"
#include "RwLevel.h"
#include "RwLevelSerializer.h"

#include "RwLevelDeserializer.h"
#include "RwLevelResourcesDAO.h"

CRwLevelManager::CRwLevelManager()
{
	m_ResourcesDAO = new CRwLevelResourcesDAO();
	m_DefaultLvlSerializer = new CRwLevelSerializer();
	m_DefaultLvlDeserializer = new CRwLevelDeserializer(new CRwLevelResourcesDAO());
}

CRwLevelManager::~CRwLevelManager()
{
	GetDeserializer()->GetResourcesDAO()->Disconnect();

	delete m_DefaultLvlSerializer;
	delete m_DefaultLvlDeserializer;
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
	level->SetDeserializer(m_DefaultLvlDeserializer);

	CRwLevelResourcesDesc rsc_desc;
	if (!rsc_desc.InitFromDir(GetRootDir(), level_name))
		return false;


	if (auto dao = dynamic_cast<CRwLevelResourcesDAO*>(GetDeserializer()->GetResourcesDAO()))
	{
		CRwLevelResourcesDesc::TWStrVec archive_paths;

		rsc_desc.GetUsedArchivePaths(archive_paths);
		dao->Connect<rw::rs::EArchiveVer::VER1>(archive_paths);
	}

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

ILevelDeserializer* CRwLevelManager::GetDeserializer() const
{
	return m_DefaultLvlDeserializer;
}

ILevelSerializer* CRwLevelManager::GetSerializer() const
{
	return m_DefaultLvlSerializer;
}

void CRwLevelManager::GetLevels(std::vector<ILevel*>& levels) const
{
	levels = m_LoadedLevels;
}