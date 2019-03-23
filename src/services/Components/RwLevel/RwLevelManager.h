#pragma once
#include <vector>

#include "../Level/ILevelManager.h"

class ILevel;
class CRwLevelDeserializer;
class CRwLevelSerializer;
class CRwLevelResourcesDAO;
class ILevelDeserializer;
class ILevelSerializer;

class CRwLevelManager : public ILevelManager
{
public:
	CRwLevelManager();
	virtual ~CRwLevelManager();

	virtual void SetRootDir(const std::wstring& dir) override;
	virtual std::wstring GetRootDir() const override;

	virtual bool LoadLevel(const std::string& level_name) override;
	virtual bool UnloadLevel(const std::string& level_name) override;
	virtual bool SaveLevel(const std::string& level_name) override;

	virtual bool UnloadAllLevels() override;
	virtual bool SaveAllLevels() override;

	virtual ILevelDeserializer* GetDeserializer() const;
	virtual ILevelSerializer* GetSerializer() const;

	void GetLevels(std::vector<ILevel*>& levels) const;

private:
	std::wstring m_RootDir;
	std::vector<ILevel*> m_LoadedLevels;

	CRwLevelResourcesDAO* m_ResourcesDAO;
	CRwLevelSerializer* m_DefaultLvlSerializer;
	CRwLevelDeserializer* m_DefaultLvlDeserializer;
};