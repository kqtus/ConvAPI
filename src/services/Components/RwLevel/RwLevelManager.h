#pragma once
#include <vector>

#include "../Level/ILevelManager.h"
#include "RwLevel.h"
#include "RwLevelSerializer.h"
#include "RwLevelDeserializer.h"

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

private:
	std::wstring m_RootDir;
	std::vector<ILevel*> m_LoadedLevels;

	CRwLevelSerializer m_DefaultLvlSerializer;
	CRwLevelDeserializer m_DefaultLvlDeserializer;
};