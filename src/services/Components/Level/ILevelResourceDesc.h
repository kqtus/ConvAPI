#pragma once
#include <string>

class ILevelResourcesDesc
{
public:
	virtual bool InitFromDir(const std::wstring& dir_name, const std::string& level_name) = 0;

	virtual ILevelResourcesDesc* Clone() const = 0;

	virtual std::string GetName() const = 0;
};