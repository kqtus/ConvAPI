#pragma once
#include "Converter.h"

#include "formats\xml\node.h"
#include "formats\rw\rwcore.h"
#include <string>
#include <sstream>

#define STR(x) std::to_string(x)

#define STR_HEX(x) [&] { \
	std::stringstream ss; \
	ss << std::hex << x; \
	return "0x" + ss.str(); \
}();

#define MAKE_NODE(name, data) new xml::node(name, STR(data))

template<>
xml::node* CConverter::From(rw::chunk_base* chunk)
{
	auto node = new xml::node();
	(*node)["type"] = rw::Decode((rw::RwCorePluginID)chunk->type);
	(*node)["size"] = STR(chunk->size);
	(*node)["version"] = STR_HEX(chunk->version);
	return node;
}

template<>
xml::node* CConverter::From(rw::plg::hanim* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("hanim");
	return node;
}

template<>
xml::node* CConverter::From(rw::plg::frame* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("frame_plg");
	node->AddChild(new xml::node("name", (const char*)chunk->name));
	return node;
}

template<>
xml::node* CConverter::From(rw::plg::bin_mesh* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("bin_mesh_plg");
	node->AddChild(MAKE_NODE("flags", chunk->flags));
	node->AddChild(MAKE_NODE("mesh_count", chunk->mesh_count));
	node->AddChild(MAKE_NODE("total_indices", chunk->total_indices));

	auto meshes_node = new xml::node("meshes");
	for (int i = 0; i < chunk->mesh_count; i++)
	{
		auto mesh_node = new xml::node("mesh");
		mesh_node->AddChild(MAKE_NODE("index_count", chunk->meshes[i].index_count));
		mesh_node->AddChild(MAKE_NODE("mat_index", chunk->meshes[i].mat_index));
		
		auto indices_node = new xml::node("indices");
		for (int j = 0; j < chunk->meshes[i].index_count; j++)
		{
			auto index_node = MAKE_NODE("index", chunk->meshes[i].indices[j]);
			(*index_node)["_i"] = STR(j);
			indices_node->AddChild(index_node);
		}
		mesh_node->AddChild(indices_node);
		meshes_node->AddChild(mesh_node);
	}
	node->AddChild(meshes_node);
	return node;
}

template<>
xml::node* CConverter::From(rw::core::frame_list_data* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("frame_list_data");
	node->AddChild(new xml::node("frame_count", STR(chunk->frame_count)));

	for (int i = 0; i < chunk->frame_count; i++)
	{
		auto info_node = new xml::node("info");

		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				info_node->AddChild(new xml::node("rotation_" + STR(j) + "_" + STR(k) + "", STR(chunk->infos[i].rotation[j][k])));
			}
		}

		info_node->AddChild(new xml::node("coords_offset.x", STR(chunk->infos[i].coords_offset.x)));
		info_node->AddChild(new xml::node("coords_offset.y", STR(chunk->infos[i].coords_offset.y)));
		info_node->AddChild(new xml::node("coords_offset.z", STR(chunk->infos[i].coords_offset.z)));
		info_node->AddChild(new xml::node("parent_frame", STR(chunk->infos[i].parent_frame)));
		info_node->AddChild(new xml::node("pad", STR(chunk->infos[i].pad)));
		node->AddChild(info_node);
	}

	return node;
}

template<>
xml::node* CConverter::From(rw::core::extension* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("extension");

	for (auto child : *chunk)
	{
		if (child->type == rw::rwID_HANIMPLG)
		{
			auto casted_child = dynamic_cast<rw::plg::hanim*>(child);
			node->AddChild(From<rw::plg::hanim*, xml::node*>(casted_child));
		}
		else if (child->type == rw::rwID_FRAMEPLG)
		{
			auto casted_child = dynamic_cast<rw::plg::frame*>(child);
			node->AddChild(From<rw::plg::frame*, xml::node*>(casted_child));
		}
		else if (child->type == rw::rwID_BINMESHPLG)
		{
			auto casted_child = dynamic_cast<rw::plg::bin_mesh*>(child);
			node->AddChild(From<rw::plg::bin_mesh*, xml::node*>(casted_child));
		}
		else
		{
			node->AddChild(From<rw::chunk_base*, xml::node*>(child));
		}
	}

	return node;
}

template<>
xml::node* CConverter::From(rw::core::clump_data* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("clump_data");
	node->AddChild(new xml::node("object_count", STR(chunk->object_count)));
	node->AddChild(new xml::node("light_count", STR(chunk->light_count)));
	node->AddChild(new xml::node("camera_count", STR(chunk->camera_count)));
	return node;
}

template<>
xml::node* CConverter::From(rw::core::frame_list* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("frame_list");
	node->AddChild(From<rw::core::frame_list_data*, xml::node*>(&chunk->data));
	
	for (auto& extension : *chunk)
	{
		node->AddChild(From<rw::core::extension*, xml::node*>(extension));
	}

	return node;
}

template<>
xml::node* CConverter::From(rw::core::material_list* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("material_list");

	return node;
}

template<>
xml::node* CConverter::From(rw::core::geometry_data* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("geometry_data");

	auto header_node = new xml::node("header");
	header_node->AddChild(MAKE_NODE("flags", chunk->header.flags));
	header_node->AddChild(MAKE_NODE("face_count", chunk->header.face_count));
	header_node->AddChild(MAKE_NODE("vertex_count", chunk->header.vertex_count));
	header_node->AddChild(MAKE_NODE("frame_count", chunk->header.frame_count));
	node->AddChild(header_node);

	if (chunk->version == 4099)
	{
		auto lighting_node = new xml::node("lighting");
		lighting_node->AddChild(MAKE_NODE("ambient", chunk->lighting.ambient));
		lighting_node->AddChild(MAKE_NODE("diffuse", chunk->lighting.diffuse));
		lighting_node->AddChild(MAKE_NODE("specular", chunk->lighting.specular));
		node->AddChild(lighting_node);
	}

	if (chunk->header.flags & rw::rpGEOMETRYPRELIT)
	{
		auto colours_node = new xml::node("colours");
		for (int i = 0; i < chunk->header.vertex_count; i++)
		{
			auto colour_node = new xml::node("colour");
			(*colour_node)["_i"] = STR(i);
			(*colour_node)["red"] = STR(chunk->colours[i].red);
			(*colour_node)["green"] = STR(chunk->colours[i].green);
			(*colour_node)["blue"] = STR(chunk->colours[i].blue);
			(*colour_node)["alpha"] = STR(chunk->colours[i].alpha);
			(*colour_node).AddChild(colour_node);
		}
		node->AddChild(colours_node);
	}

	if ((chunk->header.flags & rw::rpGEOMETRYTEXTURED) || (chunk->header.flags & rw::rpGEOMETRYTEXTURED2))
	{
		auto texture_mappings_node = new xml::node("texture_mappings");
		for (int i = 0; i < chunk->header.vertex_count; i++)
		{
			auto texture_mapping_node = new xml::node("texture_mapping");
			(*texture_mapping_node)["_i"] = STR(i);
			(*texture_mapping_node)["u"] = STR(chunk->texture_mappings[i].u);
			(*texture_mapping_node)["v"] = STR(chunk->texture_mappings[i].v);
			texture_mappings_node->AddChild(texture_mapping_node);
		}
		node->AddChild(texture_mappings_node);
	}

	if (chunk->header.flags & rw::rpGEOMETRYTEXTURED2)
	{
		auto texture_mappings_node = new xml::node("texture_mapping2");
		for (int i = 0; i < chunk->header.vertex_count; i++)
		{
			auto texture_mapping_node = new xml::node("texture_mapping");
			(*texture_mapping_node)["_i"] = STR(i);
			(*texture_mapping_node)["u"] = STR(chunk->texture_mappings2[i].u);
			(*texture_mapping_node)["v"] = STR(chunk->texture_mappings2[i].v);
			texture_mappings_node->AddChild(texture_mapping_node);
		}
		node->AddChild(texture_mappings_node);
	}

	auto faces_node = new xml::node("faces");
	for (int i = 0; i < chunk->header.face_count; i++)
	{
		auto face_node = new xml::node("face");
		(*face_node)["_i"] = STR(i);
		(*face_node)["v1"] = STR(chunk->faces[i].v1);
		(*face_node)["v2"] = STR(chunk->faces[i].v2);
		(*face_node)["v3"] = STR(chunk->faces[i].v3);
		(*face_node)["flags"] = STR(chunk->faces[i].flags);
		faces_node->AddChild(face_node);
	}
	node->AddChild(faces_node);

	auto bounding_sphere_node = new xml::node("bounding_sphere");
	bounding_sphere_node->AddChild(MAKE_NODE("pos.x", (float)chunk->bounding_sphere.pos.x));
	bounding_sphere_node->AddChild(MAKE_NODE("pos.y", (float)chunk->bounding_sphere.pos.y));
	bounding_sphere_node->AddChild(MAKE_NODE("pos.z", (float)chunk->bounding_sphere.pos.z));
	bounding_sphere_node->AddChild(MAKE_NODE("radius", (float)chunk->bounding_sphere.radius));
	node->AddChild(bounding_sphere_node);

	auto flags_node = new xml::node("flags");
	flags_node->AddChild(MAKE_NODE("has_position", chunk->flags.has_position));
	flags_node->AddChild(MAKE_NODE("has_normals", chunk->flags.has_normals));
	node->AddChild(flags_node);

	auto vertices_node = new xml::node("vertices");
	for (int i = 0; i < chunk->header.vertex_count; i++)
	{
		auto vertex_node = new xml::node("vertex");
		(*vertex_node)["_i"] = STR(i);
		(*vertex_node)["x"] = STR(chunk->vertices[i].x);
		(*vertex_node)["y"] = STR(chunk->vertices[i].y);
		(*vertex_node)["z"] = STR(chunk->vertices[i].z);
		vertices_node->AddChild(vertex_node);
	}
	node->AddChild(vertices_node);

	if (chunk->header.flags & rw::rpGEOMETRYNORMALS)
	{
		auto normals_node = new xml::node("normals");
		for (int i = 0; i < chunk->header.vertex_count; i++)
		{
			auto normal_node = new xml::node("normal");
			(*normal_node)["_i"] = STR(i);
			(*normal_node)["x"] = STR(chunk->normals[i].x);
			(*normal_node)["y"] = STR(chunk->normals[i].y);
			(*normal_node)["z"] = STR(chunk->normals[i].z);
			normals_node->AddChild(normal_node);
		}
		node->AddChild(normals_node);
	}

	return node;
}

template<>
xml::node* CConverter::From(rw::core::geometry* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("geometry");
	node->AddChild(From<rw::core::geometry_data*, xml::node*>(&chunk->data));
	node->AddChild(From<rw::core::material_list*, xml::node*>(&chunk->materials));
	node->AddChild(From<rw::core::extension*, xml::node*>(&chunk->ext));
	return node;
}

template<>
xml::node* CConverter::From(rw::core::geometry_list_data* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("geometry_list_data");
	node->AddChild(new xml::node("geometry_count", STR(chunk->geometry_count)));
	return node;
}

template<>
xml::node* CConverter::From(rw::core::geometry_list* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("geometry_list");
	node->AddChild(From<rw::core::geometry_list_data*, xml::node*>(&chunk->data));
	
	for (auto& geometry : *chunk)
	{
		node->AddChild(From<rw::core::geometry*, xml::node*>(geometry));
	}
	return node;
}

template<>
xml::node* CConverter::From(rw::core::atomic_data* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("atomic_data");
	node->AddChild(MAKE_NODE("frame_index", chunk->frame_index));
	node->AddChild(MAKE_NODE("geometry_index", chunk->geometry_index));
	node->AddChild(MAKE_NODE("flags", chunk->flags));
	node->AddChild(MAKE_NODE("pad", chunk->pad));
	return node;
}

template<>
xml::node* CConverter::From(rw::core::atomic* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("atomic");
	node->AddChild(From<rw::core::atomic_data*, xml::node*>(&chunk->data));
	node->AddChild(From<rw::core::extension*, xml::node*>(&chunk->ext));
	return node;
}

template<>
xml::node* CConverter::From(rw::core::clump* chunk)
{
	auto node = From<rw::chunk_base*, xml::node*>(chunk);
	node->SetName("clump");
	node->AddChild(From<rw::core::clump_data*, xml::node*>(&chunk->data));
	node->AddChild(From<rw::core::frame_list*, xml::node*>(&chunk->frames));
	node->AddChild(From<rw::core::geometry_list*, xml::node*>(&chunk->geometries));

	for (auto& atomic : chunk->atomics)
	{
		node->AddChild(From<rw::core::atomic*, xml::node*>(&atomic));
	}

	node->AddChild(From<rw::core::extension*, xml::node*>(&chunk->ext));
	node->UpdateIndentsForSubnodes();
	return node;
}