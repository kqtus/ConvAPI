#include "RwLevelDeserializer.h"
#include "RwLevelResourcesDAO.h"
#include "RwLevelResourcesDesc.h"
#include <algorithm>
#include <map>
#include "../../formats/rw/rwrs.h"

CRwLevelDeserializer::CRwLevelDeserializer(CRwLevelResourcesDAO* rsc_dao)
	: m_RscDAO(rsc_dao)
{
}

bool CRwLevelDeserializer::Deserialize(ILevelResourcesDesc* rsc_desc, TLevelEntitiyPtrs& out_entities)
{
	auto rw_rsc_desc = dynamic_cast<CRwLevelResourcesDesc*>(rsc_desc);
	auto dao = dynamic_cast<CRwLevelResourcesDAO*>(GetResourcesDAO());

	if (!rw_rsc_desc)
		return false;

	if (!dao)
		return false;

	CRwLevelResourcesDesc::TWStrVec paths;
	rw_rsc_desc->GetItemDefinitionFilePaths(paths);

	std::map<std::string, rw::core::clump*> models;

	for (auto ide_path : paths)
	{
		in_stream<EStreamType::TEXT> ide_strm;
		if (!ide_strm.Open(ide_path.c_str()))
			continue;
		
		rw::rs::item_definitions ide;
		if (ide.Read(ide_strm))
		{
			for (auto& obj_entry : ide.GetObjEntries())
			{
				auto mdl_name = obj_entry.model_name + ".dff";

				if (auto obj_model = dao->GetModel(mdl_name))
					models[mdl_name] = obj_model;
			}

			for (auto& tobj_entry : ide.GetTobjEntries())
			{
				auto mdl_name = tobj_entry.model_name + ".dff";

				if (auto tobj_model = dao->GetModel(mdl_name))
					models[mdl_name] = tobj_model;
			}
		}

		ide_strm.Close();
	}

	paths.clear();
	rw_rsc_desc->GetItemPlacementFilePaths(paths);

	for (auto& ipl_path : paths)
	{
		in_stream<EStreamType::TEXT> ipl_strm;
		if (!ipl_strm.Open(ipl_path.c_str()))
			continue;

		rw::rs::item_placements ipl;
		if (ipl.Read(ipl_strm))
		{
			for (auto& inst_entry : ipl.GetInstEntries())
			{
				auto itr = models.find(inst_entry.model_name + ".dff");
				if (itr != models.end())
				{
					auto pos_msh = new SPositionedMesh();
					pos_msh->mesh = itr->second;
					pos_msh->pos = vec3<float>(inst_entry.pos_x, inst_entry.pos_z, inst_entry.pos_y);
					pos_msh->rot = vec3<float>(inst_entry.rot_x, inst_entry.rot_y, inst_entry.rot_z);

					m_Entities.push_back(pos_msh);
				}
			}
		}

		ipl_strm.Close();
	}

	return true;
}


ILevelResourcesDAO* CRwLevelDeserializer::GetResourcesDAO() const
{
	return m_RscDAO;
}

void CRwLevelDeserializer::GetLoadedEntities(std::vector<SPositionedMesh*>& entities) const
{
	entities = m_Entities;
}