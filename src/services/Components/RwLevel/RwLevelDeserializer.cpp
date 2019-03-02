#include "../Level/ILevelResourceDesc.h"
#include "RwLevelDeserializer.h"
#include "RwLevelResourcesDesc.h"
#include "../../formats/rw/rwrs.h"

bool CRwLevelDeserializer::Deserialize(ILevelResourcesDesc* rsc_desc, TLevelEntitiyPtrs& out_entities)
{
	auto rw_rsc_desc = dynamic_cast<CRwLevelResourcesDesc*>(rsc_desc);

	if (!rw_rsc_desc)
		return false;

	CRwLevelResourcesDesc::TWStrVec paths;
	rw_rsc_desc->GetItemDefinitionFilePaths(paths);

	for (auto& ide_path : paths)
	{
		in_stream<EStreamType::TEXT> ide_strm;
		if (!ide_strm.Open(ide_path.c_str()))
			continue;

		ide_strm.Close();
	}

	rw_rsc_desc->GetItemPlacementFilePaths(paths);

	for (auto& ipl_path : paths)
	{
		in_stream<EStreamType::TEXT> ipl_strm;
		if (!ipl_strm.Open(ipl_path.c_str()))
			continue;

		ipl_strm.Close();
	}

	return false;
}