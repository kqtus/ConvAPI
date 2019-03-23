#pragma once
#include <vector>

#include "ILevelEntity.h"

class ILevelResourcesDesc;
class ILevelResourcesDAO;

class ILevelDeserializer
{
public:
	virtual bool Deserialize(ILevelResourcesDesc* rsc_desc, TLevelEntitiyPtrs& out_entities) = 0;
	virtual ILevelResourcesDAO* GetResourcesDAO() const = 0;
};