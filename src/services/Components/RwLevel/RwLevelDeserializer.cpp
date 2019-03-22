#include "../Level/ILevelResourceDesc.h"
#include "RwLevelDeserializer.h"
#include "RwLevelResourcesDesc.h"
#include <algorithm>
#include "../../formats/rw/rwrs.h"

bool CRwLevelDeserializer::Deserialize(ILevelResourcesDesc* rsc_desc, TLevelEntitiyPtrs& out_entities)
{
	auto rw_rsc_desc = dynamic_cast<CRwLevelResourcesDesc*>(rsc_desc);

	if (!rw_rsc_desc)
		return false;

	CRwLevelResourcesDesc::TWStrVec paths;
	rw_rsc_desc->GetItemDefinitionFilePaths(paths);

	for (auto ide_path : paths)
	{
		in_stream<EStreamType::TEXT> ide_strm;
		if (!ide_strm.Open(ide_path.c_str()))
			continue;
			
		rw::rs::item_definitions ide;
		if (ide.Read(ide_strm))
		{
			
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

		}

		ipl_strm.Close();
	}

	return true;
}