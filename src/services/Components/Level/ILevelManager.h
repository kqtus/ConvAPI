#pragma once
#include <string>

class ILevelManager
{
public:
	virtual bool LoadLevel(const std::string& level_name) = 0;
	virtual bool UnloadLevel(const std::string& level_name) = 0;
	virtual bool SaveLevel(const std::string& level_name) = 0;

	virtual bool UnloadAllLevels() = 0;
	virtual bool SaveAllLevels() = 0;
};