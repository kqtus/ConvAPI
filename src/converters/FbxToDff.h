#pragma once
#include "Converter.h"

#include "../formats/rw/rwcore.h"
#include "fbxsdk.h"
#include <set>
#include <math.h>

template<>
rw::core::clump_data* CConverter::From(FbxNode* node)
{
	auto data = new rw::core::clump_data();
	data->object_count = node->GetChildCount();
	data->light_count = 0;
	data->camera_count = 0;
	return data;
}

template<>
rw::core::frame_list_data* CConverter::From(FbxNode* node)
{
	auto data = new rw::core::frame_list_data();

	data->frame_count = node->GetChildCount();
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

	std::string name(node->GetName());

	frame_plg->size = name.size();
	INIT_ARR(frame_plg->name, frame_plg->size);
	std::copy(name.begin(), name.end(), frame_plg->name);
	frame_plg->name[frame_plg->size] = '\0';
	return frame_plg;
}

template<>
rw::core::frame_list* CConverter::From(FbxNode* node)
{
	auto frames = new rw::core::frame_list();
	frames->data = *From<FbxNode*, rw::core::frame_list_data*>(node);

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
		}
	}

	return frames;
}

template<>
rw::core::geometry_list_data* CConverter::From(FbxNode* node)
{
	auto geometry_data = new rw::core::geometry_list_data();
	geometry_data->geometry_count = node->GetChildCount();

	return geometry_data;
}

template<>
rw::core::geometry_data* CConverter::From(FbxNode* node)
{
	auto data = new rw::core::geometry_data();
	auto mesh = (FbxMesh*)(node->GetNodeAttribute());
	
	data->header.flags = 0x2F & ~rw::rpGEOMETRYPRELIT & ~rw::rpGEOMETRYTEXTURED;
	data->header.frame_count = 1;
	data->header.face_count = 0;
	data->header.vertex_count = mesh->GetControlPointsCount();

	if (data->header.flags & rw::rpGEOMETRYPRELIT)
	{
		INIT_ARR(data->colours, data->header.vertex_count);
		for (int i = 0; i < data->header.vertex_count; i++)
		{

		}
	}

	if ((data->header.flags & rw::rpGEOMETRYTEXTURED) || (data->header.flags & rw::rpGEOMETRYTEXTURED2))
	{
		INIT_ARR(data->texture_mappings, data->header.vertex_count);
	}

	if (data->header.flags & rw::rpGEOMETRYTEXTURED2)
	{
		INIT_ARR(data->texture_mappings2, data->header.vertex_count);
	}

	int poly_count = mesh->GetPolygonCount();
	int* polygonVertices = mesh->GetPolygonVertices();

	for (int i = 0; i < poly_count; i++)
	{
		int start_index = mesh->GetPolygonVertexIndex(i);

	}

	const int FACE_VERTEX_COUNT = 3;
	using TFace = rw::core::geometry_data::face;

	std::vector<TFace> faces;

	int polygon_count = mesh->GetPolygonCount();
	for (int i = 0; i < polygon_count; i++)
	{
		int polygon_size = mesh->GetPolygonSize(i);
		for (int j = 0; j < polygon_size; j += FACE_VERTEX_COUNT)
		{
			uint16_t v1 = mesh->GetPolygonVertex(i, j);
			uint16_t v2 = mesh->GetPolygonVertex(i, (j + 1) % polygon_size);
			uint16_t v3 = mesh->GetPolygonVertex(i, (j + 2) % polygon_size);
			uint16_t flags = 0; // What is this responsible for??

			faces.push_back(TFace { v2, v1, flags, v3 } );
		}
	}

	data->header.face_count = faces.size();
	INIT_ARR(data->faces, data->header.face_count);
	std::copy(faces.begin(), faces.end(), data->faces);

	mesh->ComputeBBox();
	
	auto bbox_max = mesh->BBoxMax.EvaluateValue();
	auto bbox_min = mesh->BBoxMin.EvaluateValue();
	auto vertices = mesh->GetControlPoints();

	data->bounding_sphere.pos.x = (bbox_max[0] + bbox_min[0]) / 2;
	data->bounding_sphere.pos.y = (bbox_max[1] + bbox_min[1]) / 2;
	data->bounding_sphere.pos.z = (bbox_max[2] + bbox_min[2]) / 2;

	data->bounding_sphere.radius = sqrt(
		pow(abs(data->bounding_sphere.pos.x - bbox_max[0]), 2) +
		pow(abs(data->bounding_sphere.pos.y - bbox_max[1]), 2) +
		pow(abs(data->bounding_sphere.pos.z - bbox_max[2]), 2)
	);

	data->flags.has_position = 0;
	data->flags.has_normals = 0;

	INIT_ARR(data->vertices, data->header.vertex_count);
	for (int i = 0; i < data->header.vertex_count; i++)
	{
		data->vertices[i].x = (float_t)vertices[i][0];
		data->vertices[i].y = (float_t)vertices[i][1];
		data->vertices[i].z = (float_t)vertices[i][2];
	}

	if (data->header.flags & rw::rpGEOMETRYNORMALS)
	{
		INIT_ARR(data->normals, data->header.vertex_count);
	}

	return data;
}

template<>
rw::core::material_list_data* CConverter::From(FbxNode* node)
{
	auto data = new rw::core::material_list_data();
	data->material_count = 0;//node->GetMaterialCount();

	INIT_ARR(data->material_indices, data->material_count);
	for (int i = 0; i < data->material_count; i++)
	{
		data->material_indices[i] = -1;
	}

	return data;
}

template<>
rw::core::texture_data* CConverter::From(FbxNode* node)
{
	auto tex_data = new rw::core::texture_data();

	return tex_data;
}

template<>
rw::string* CConverter::From(FbxNode* node)
{
	auto str = new rw::string();
	
	return str;
}

template<>
rw::core::texture* CConverter::From(FbxNode* node)
{
	auto tex = new rw::core::texture();

	tex->data = *From<FbxNode*, rw::core::texture_data*>(node);
	tex->texture_name = *From<FbxNode*, rw::string*>(node);
	tex->mask_name = *From<FbxNode*, rw::string*>(node);

	return tex;
}

template<>
rw::core::material_data* CConverter::From(FbxNode* node)
{
	auto mat_data = new rw::core::material_data();
	mat_data->pad1 = 0;
	mat_data->texture_count = 0;
	mat_data->color = rgba{ 255, 255, 255, 255 };
	mat_data->pad2 = 0;
	mat_data->unk_vec.x = 1.0f;
	mat_data->unk_vec.y = 0.0f;
	mat_data->unk_vec.z = 1.0f;
	return mat_data;
}

template<>
rw::core::material* CConverter::From(FbxNode* node)
{
	auto material = new rw::core::material();
	material->data = *From<FbxNode*, rw::core::material_data*>(node);

	for (int i = 0; i < material->data.texture_count; i++)
	{
		auto tex = From<FbxNode*, rw::core::texture*>(node);
		material->push_back(tex);
	}
	return material;
}

template<>
rw::core::material_list* CConverter::From(FbxNode* node)
{
	auto materials = new rw::core::material_list();
	materials->data = *From<FbxNode*, rw::core::material_list_data*>(node);

	for (int i = 0; i < materials->data.material_count; i++)
	{
		auto child = node->GetChild(i);
		auto material = From<FbxNode*, rw::core::material*>(child);
		materials->push_back(material);
	}
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

	for (int i = 0; i < geometries->data.geometry_count; i++)
	{
		auto child = node->GetChild(i);
		auto attr_type = child->GetNodeAttribute()->GetAttributeType();

		if (attr_type == FbxNodeAttribute::eMesh)
		{
			auto geometry = From<FbxNode*, rw::core::geometry*>(child);
			geometries->push_back(geometry);
		}
	}
	return geometries;
}

template<>
rw::core::atomic_data* CConverter::From(FbxNode* node, int num)
{
	auto data = new rw::core::atomic_data();
	data->frame_index = num;
	data->geometry_index = num;
	data->flags = 0x01 | 0x04;
	return data;
}

template<>
rw::core::atomic* CConverter::From(FbxNode* node, int num)
{
	auto atomic = new rw::core::atomic();
	atomic->data = *From<FbxNode*, rw::core::atomic_data*>(node, num);
	return atomic;
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

	for (int i = 0; i < clump->data.object_count; i++)
	{
		auto atm = *From<FbxNode*, rw::core::atomic*>(node, i);
		clump->atomics.push_back(atm);
	}

	clump->UpdateSize();
	return clump;
}