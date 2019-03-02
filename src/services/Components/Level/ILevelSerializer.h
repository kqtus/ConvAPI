#pragma once

class ILevelResourcesDesc;

class ILevelSerializer
{
public:
	virtual bool Serialize(ILevelResourcesDesc* rsc_desc) = 0;
};