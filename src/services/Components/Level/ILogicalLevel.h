#pragma once
#include <functional>
#include <vector>

#include "ILevelEntity.h"
#include "ILevelObjectsFilter.h"


class ILogicalLevel
{
public:
	virtual bool GetObjects(const ILevelObjectsFilter& filter, std::vector<ILevelEntity*> out_entities) = 0;

	virtual bool AddObject(ILevelEntity* entity) = 0;
	virtual bool RemoveObject(ILevelEntity* entity) = 0;
	virtual bool FindObject(std::function<bool(ILevelEntity*)> pred, ILevelEntity*& out_obj) = 0;
};
