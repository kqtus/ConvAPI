#pragma once
#include "ILogicalLevel.h"
#include "IPhysicalLevel.h"

struct SLevelStats
{
	bool level_loaded;
	int all_objects = 0;
	int static_buildings = 0;
	int lod_buildings = 0;
};

class ILevel : public ILogicalLevel, public IPhysicalLevel
{
public:
	virtual void GetStats(SLevelStats& stats) const = 0;
};