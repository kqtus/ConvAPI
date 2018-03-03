#pragma once
#include "Converter.h"

#include "formats/rw/rwcore.h"
#include "fbxsdk.h"

template<>
rw::core::clump_data* CConverter::From(FbxNode* node)
{
	auto data = new rw::core::clump_data();
	data->object_count = 1;
	return data;
}

template<>
rw::core::frame_list_data* CConverter::From(FbxNode* node)
{
	auto data = new rw::core::frame_list_data();

	data->frame_count = 1;
	INIT_ARR(data->infos, data->frame_count);

	for (int i = 0; i < data->frame_count; i++)
	{
		auto& info = data->infos[i];

		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++)
			{
				if (j == k)
				{
					info.rotation[j][k] = 1.0f;
				}
				else
				{
					info.rotation[j][k] = 0.0f;
				}
			}
		}

		info.coords_offset.x = 0.0f;
		info.coords_offset.y = 0.0f;
		info.coords_offset.z = 0.0f;
		info.parent_frame = (1 << 31);
		info.pad = 0;
	}
	return data;
}

template<>
rw::plg::frame* CConverter::From(FbxNode* node)
{
	auto frame_plg = new rw::plg::frame();
	const int name_size_aligned = 24;

	frame_plg->name = new uint8_t[name_size_aligned];
	strncpy((char*)frame_plg->name, node->GetName(), name_size_aligned);

	return frame_plg;
}

template<>
rw::core::frame_list* CConverter::From(FbxNode* node)
{
	auto frames = new rw::core::frame_list();
	frames->data = *From<FbxNode*, rw::core::frame_list_data*>(node);

	for (int i = 0; i < node->GetChildCount(); i++)
	{
		auto child = node->GetChild(i);
		auto attr_type = child->GetNodeAttribute()->GetAttributeType();

		if (attr_type == FbxNodeAttribute::eMesh)
		{
			auto ext = new rw::core::extension();
			ext->push_back(From<FbxNode*, rw::plg::frame*>(child));
			frames->push_back(ext);
		}
	}

	return frames;
}

template<>
rw::core::geometry_list* CConverter::From(FbxNode* node)
{
	auto geometries = new rw::core::geometry_list();
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		auto child = node->GetChild(i);
		auto attr_type = child->GetNodeAttribute()->GetAttributeType();

		if (attr_type == FbxNodeAttribute::eMesh)
		{

		}
	}
	return geometries;
}

template<>
rw::core::clump* CConverter::From(FbxNode* node)
{
	auto clump = new rw::core::clump();
	//clump->type = rw::rwID_CLUMP;
	// #TODO Redesign dff structures to hold pointers
	// and fix this code (mem leaks guaranteed)
	clump->data = *From<FbxNode*, rw::core::clump_data*>(node);
	clump->frames = *From<FbxNode*, rw::core::frame_list*>(node);
	clump->geometries = *From<FbxNode*, rw::core::geometry_list*>(node);

	clump->size = (
		clump->data.size +
		clump->frames.chunk_base::size +
		clump->geometries.chunk_base::size
	);

	return clump;
}






