#pragma once
#include "ILevelEntity.h"

class ILevelObjectsFilter
{
public:
	virtual bool Conforms(ILevelEntity* entity) const = 0;
};

