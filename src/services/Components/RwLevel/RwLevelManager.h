#pragma once
#include "../Level/ILevelManager.h"

class CRwLevelManager : public ILevelManager
{
public:
	CRwLevelManager();
	virtual ~CRwLevelManager();

	virtual bool LoadLevel(const std::string& level_name) override;
	virtual bool UnloadLevel(const std::string& level_name) override;
	virtual bool SaveLevel(const std::string& level_name) override;

	virtual bool UnloadAllLevels() override;
	virtual bool SaveAllLevels() override;
};