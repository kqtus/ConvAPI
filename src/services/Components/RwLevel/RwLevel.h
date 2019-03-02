#pragma once
#include <vector>
#include <algorithm>

#include "../Level/LevelBase.h"

class CRwLevel : public CLevelBase
{
public:
	CRwLevel();
	virtual ~CRwLevel();

	/** ILevel Interface **/
	virtual void GetStats(SLevelStats& stats) const override;
	/** End of ILevel Interface**/

	/** ILogicalLevel Interface **/
	virtual bool GetObjects(const ILevelObjectsFilter& filter, std::vector<ILevelEntity*> out_entities) override;

	virtual bool AddObject(ILevelEntity* entity) override;
	virtual bool RemoveObject(ILevelEntity* entity) override;
	virtual bool FindObject(std::function<bool(ILevelEntity*)> pred, ILevelEntity*& out_obj) override;
	/** End of ILogicalLevel interface**/

	/** IPhysicalLevel Interface **/
	virtual bool Load(ILevelResourcesDesc* rsc_desc, ILevelDeserializer* deserializer = nullptr) override;
	virtual bool Save(ILevelSerializer* serializer = nullptr) override;
	virtual bool SaveAs(ILevelResourcesDesc* rsc_desc, ILevelSerializer* serializer = nullptr) override;
	/** End of IPhysicalLevel Interface **/

	ILevelResourcesDesc* GetCurrLoadedRscDesc() const;

protected:
	std::vector<ILevelEntity*> m_LevelEntities;
	ILevelResourcesDesc* m_LoadedLvlRscDesc = nullptr;
};