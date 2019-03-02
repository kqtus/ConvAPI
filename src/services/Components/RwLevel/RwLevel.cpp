#include "RwLevel.h"
#include "../Level/ILevelDeserializer.h"
#include "../Level/ILevelSerializer.h"
#include "../Level/ILevelResourceDesc.h"

CRwLevel::CRwLevel()
{
}

CRwLevel::~CRwLevel()
{
	delete m_LoadedLvlRscDesc;
}

void CRwLevel::GetStats(SLevelStats& stats) const
{

}

bool CRwLevel::GetObjects(const ILevelObjectsFilter& filter, std::vector<ILevelEntity*> out_entities)
{
	for (auto& entity : m_LevelEntities)
	{
		if (filter.Conforms(entity))
			out_entities.push_back(entity);
	}

	return true;
}

bool CRwLevel::AddObject(ILevelEntity* entity)
{
	if (std::find(m_LevelEntities.begin(), m_LevelEntities.end(), entity) != m_LevelEntities.end())
	{
		m_LevelEntities.push_back(entity);
		return true;
	}
	return false;
}

bool CRwLevel::RemoveObject(ILevelEntity* entity)
{
	if (std::find(m_LevelEntities.begin(), m_LevelEntities.end(), entity) != m_LevelEntities.end())
	{
		std::remove(m_LevelEntities.begin(), m_LevelEntities.end(), entity);
		return true;
	}
	return false;
}

bool CRwLevel::FindObject(std::function<bool(ILevelEntity*)> pred, ILevelEntity*& out_obj)
{
	auto itr = std::find_if(m_LevelEntities.begin(), m_LevelEntities.end(), pred);
	if (itr == m_LevelEntities.end())
		return false;

	out_obj = *itr;
	return true;
}

bool CRwLevel::Load(ILevelResourcesDesc* rsc_desc, ILevelDeserializer* deserializer)
{
	auto _deserializer = deserializer ? deserializer : GetDeserializer();

	if (!_deserializer)
		return false;

	bool res = _deserializer->Deserialize(rsc_desc, m_LevelEntities);

	if (res)
		m_LoadedLvlRscDesc = rsc_desc->Clone();

	return res;
}

bool CRwLevel::Save(ILevelSerializer* serializer)
{
	return SaveAs(GetCurrLoadedRscDesc(), serializer);
}

bool CRwLevel::SaveAs(ILevelResourcesDesc* rsc_desc, ILevelSerializer* serializer)
{
	if (!rsc_desc)
		return false;

	auto _serializer = serializer ? serializer : GetSerializer();

	if (!_serializer)
		return false;

	return _serializer->Serialize(rsc_desc);
}

ILevelResourcesDesc* CRwLevel::GetCurrLoadedRscDesc() const
{
	return m_LoadedLvlRscDesc;
}