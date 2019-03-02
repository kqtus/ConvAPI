#pragma once
#include <vector>

#include "ILevelEntity.h"

class ILevelResourcesDesc;

class ILevelDeserializer
{
public:
	virtual bool Deserialize(ILevelResourcesDesc* rsc_desc, TLevelEntitiyPtrs& out_entities) = 0;
};