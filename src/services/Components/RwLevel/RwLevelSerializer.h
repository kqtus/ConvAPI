#pragma once
#include "../Level/ILevelSerializer.h"

class CRwLevelSerializer : public ILevelSerializer
{
public:
	virtual bool Serialize(ILevelResourcesDesc* rsc_desc) override;
};