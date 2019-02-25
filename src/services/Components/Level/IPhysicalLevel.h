#pragma once
#include <string>

class ILevelResourcesDesc
{
};

class IPhysicalLevel
{
public:
	virtual bool Load(ILevelResourcesDesc* rsc_desc) = 0;
	virtual bool Save() = 0;
	virtual bool SaveAs(const std::string& level_name, ILevelResourcesDesc* out_rsc_desc = nullptr) = 0;
};