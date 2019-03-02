#pragma once
#include "../Level/ILevelDeserializer.h"

class CRwLevelDeserializer : public ILevelDeserializer
{
public:
	virtual bool Deserialize(ILevelResourcesDesc* rsc_desc, TLevelEntitiyPtrs& out_entities) override;
};