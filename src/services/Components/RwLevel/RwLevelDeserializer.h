#pragma once
#include "../Level/ILevelDeserializer.h"

// Temporary
#include "../../../formats/rw/rwcore.h"

class CRwLevelResourcesDAO;

struct SPositionedMesh
{
	rw::core::clump* mesh;
	vec3<float> pos;
	vec3<float> rot;
};

class CRwLevelDeserializer : public ILevelDeserializer
{
public:
	CRwLevelDeserializer(CRwLevelResourcesDAO* rsc_dao);

	virtual bool Deserialize(ILevelResourcesDesc* rsc_desc, TLevelEntitiyPtrs& out_entities) override;
	virtual ILevelResourcesDAO* GetResourcesDAO() const override;

	void GetLoadedEntities(std::vector<SPositionedMesh*>& entities) const;

private:
	ILevelResourcesDAO* m_RscDAO;

	std::vector<SPositionedMesh*> m_Entities;
};