#pragma once
#include <string>

class ILevelManager
{
public:
	virtual void SetRootDir(const std::wstring& dir) = 0;
	virtual std::wstring GetRootDir() const = 0;

	virtual bool LoadLevel(const std::string& level_name) = 0;
	virtual bool UnloadLevel(const std::string& level_name) = 0;
	virtual bool SaveLevel(const std::string& level_name) = 0;

	virtual bool UnloadAllLevels() = 0;
	virtual bool SaveAllLevels() = 0;
};