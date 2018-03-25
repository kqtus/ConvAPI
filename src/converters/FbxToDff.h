#pragma once
#include "Converter.h"

#include "../formats/rw/rwcore.h"
#include "fbxsdk.h"
#include <set>

const static int RW_HEADER_SIZE = 12;

template<>
rw::core::clump_data* CConverter::From(FbxNode* node)
{
	auto data = new rw::core::clump_data();
	data->object_count = node->GetChildCount();
	data->light_count = 0;
	data->camera_count = 0;
	data->size = sizeof(data->object_count) + sizeof(data->light_count) + sizeof(data->camera_count);
	return data;
}

template<>
rw::core::frame_list_data* CConverter::From(FbxNode* node)
{
	auto data = new rw::core::frame_list_data();

	data->frame_count = node->GetChildCount();
	INIT_ARR(data->infos, data->frame_count);
	data->size += sizeof(data->frame_count);

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
				data->size += sizeof(info.rotation[j][k]);
			}
		}

		info.coords_offset.x = 0.0f;
		info.coords_offset.y = 0.0f;
		info.coords_offset.z = 0.0f;
		info.parent_frame = (1 << 31);
		info.pad = 0;

		data->size += sizeof(info.coords_offset.x);
		data->size += sizeof(info.coords_offset.y);
		data->size += sizeof(info.coords_offset.z);
		data->size += sizeof(info.parent_frame);
		data->size += sizeof(info.pad);
	}

	return data;
}

template<>
rw::plg::frame* CConverter::From(FbxNode* node)
{
	auto frame_plg = new rw::plg::frame();
	frame_plg->size = strlen(node->GetName());

	frame_plg->name = new uint8_t[frame_plg->size];
	strncpy((char*)frame_plg->name, node->GetName(), frame_plg->size);

	return frame_plg;
}

template<>
rw::core::frame_list* CConverter::From(FbxNode* node)
{
	auto frames = new rw::core::frame_list();
	frames->data = *From<FbxNode*, rw::core::frame_list_data*>(node);

	((rw::chunk_base*)frames)->size += frames->data.size + RW_HEADER_SIZE;

	for (int i = 0; i < frames->data.frame_count; i++)
	{
		auto child = node->GetChild(i);
		auto attr_type = child->GetNodeAttribute()->GetAttributeType();

		if (attr_type == FbxNodeAttribute::eMesh)
		{
			auto ext = new rw::core::extension();
			auto frame = From<FbxNode*, rw::plg::frame*>(child);

			ext->push_back(frame);
			frames->push_back(ext);

			((rw::chunk_base*)ext)->size += frame->size + RW_HEADER_SIZE;
			((rw::chunk_base*)frames)->size += ((rw::chunk_base*)ext)->size + RW_HEADER_SIZE;
		}
	}

	return frames;
}

template<>
rw::core::geometry_list_data* CConverter::From(FbxNode* node)
{
	auto geometry_data = new rw::core::geometry_list_data();
	geometry_data->geometry_count = node->GetChildCount();
	geometry_data->size += sizeof(geometry_data->geometry_count);
	return geometry_data;
}

template<>
rw::core::geometry_data* CConverter::From(FbxNode* node)
{
	auto data = new rw::core::geometry_data();
	data->header.flags = 0x2F;
	FbxMesh* mesh = (FbxMesh*)node->GetNodeAttribute();
	
	int poly_count = mesh->GetPolygonCount();
	int* vertices = mesh->GetPolygonVertices();

	for (int i = 0; i < poly_count; i++)
	{
		int start_index = mesh->GetPolygonVertexIndex(i);
		
	}

	data->header.frame_count = 1;
	return data;
}

template<>
rw::core::material_list* CConverter::From(FbxNode* node)
{
	auto materials = new rw::core::material_list();
	
	return materials;
}

template<>
rw::core::geometry* CConverter::From(FbxNode* node)
{
	auto geometry = new rw::core::geometry();
	geometry->data = *From<FbxNode*, rw::core::geometry_data*>(node);
	geometry->materials = *From<FbxNode*, rw::core::material_list*>(node);
	return geometry;
}

template<>
rw::core::geometry_list* CConverter::From(FbxNode* node)
{
	auto geometries = new rw::core::geometry_list();
	geometries->data = *From<FbxNode*, rw::core::geometry_list_data*>(node);
	((rw::chunk_base*)geometries)->size += geometries->data.size + RW_HEADER_SIZE;

	for (int i = 0; i < geometries->data.geometry_count; i++)
	{
		auto child = node->GetChild(i);
		auto attr_type = child->GetNodeAttribute()->GetAttributeType();

		if (attr_type == FbxNodeAttribute::eMesh)
		{
			auto geometry = From<FbxNode*, rw::core::geometry*>(node->GetChild(i));
			geometries->push_back(geometry);
			((rw::chunk_base*)geometries)->size += geometry->size + RW_HEADER_SIZE;
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
		RW_HEADER_SIZE +
		clump->data.size +
		RW_HEADER_SIZE +
		clump->frames.chunk_base::size +
		RW_HEADER_SIZE +
		clump->geometries.chunk_base::size
	);

	return clump;
}






