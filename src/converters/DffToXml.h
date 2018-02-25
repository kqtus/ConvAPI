#pragma once
#include "Converter.h"

#include "formats\xml\node.h"
#include "formats\rw\rwcore.h"
#include <string>

#define STR(x) std::to_string(x)
#define MAKE_NODE(name, data) xml::node(name, STR(data))

template<>
xml::node CConverter::From(const rw::chunk_base& chunk)
{
	xml::node node;
	node["type"] = rw::Decode((rw::RwCorePluginID)chunk.type);
	node["size"] = STR(chunk.size);
	node["version"] = STR(chunk.version);
	return node;
}

template<>
xml::node CConverter::From(const rw::plg::hanim& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("hanim");
	return node;
}

template<>
xml::node CConverter::From(const rw::plg::frame& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("frame_plg");
	node.AddChild(xml::node("name", (const char*)chunk.name));
	return node;
}

template<>
xml::node CConverter::From(const rw::core::frame_list_data& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("frame_list_data");
	node.AddChild(xml::node("frame_count", STR(chunk.frame_count)));

	for (int i = 0; i < chunk.frame_count; i++)
	{
		auto info_node = xml::node("info");

		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				info_node.AddChild(xml::node("rotation_" + STR(j) + "_" + STR(k) + "", STR(chunk.infos[i].rotation[j][k])));
			}
		}

		info_node.AddChild(xml::node("coords_offset.x", STR(chunk.infos[i].coords_offset.x)));
		info_node.AddChild(xml::node("coords_offset.y", STR(chunk.infos[i].coords_offset.y)));
		info_node.AddChild(xml::node("coords_offset.z", STR(chunk.infos[i].coords_offset.z)));
		info_node.AddChild(xml::node("parent_frame", STR(chunk.infos[i].parent_frame)));
		info_node.AddChild(xml::node("pad", STR(chunk.infos[i].pad)));
		node.AddChild(info_node);
	}

	return node;
}

template<>
xml::node CConverter::From(const rw::core::frame_list_ext& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("frame_list_ext");
	node.AddChild(From<rw::plg::hanim, xml::node>(chunk.hanim_plg));
	node.AddChild(From<rw::plg::frame, xml::node>(chunk.frame_plg));
	return node;
}

template<>
xml::node CConverter::From(const rw::core::clump_data& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("clump_data");
	node.AddChild(xml::node("object_count", STR(chunk.object_count)));
	node.AddChild(xml::node("pad1", STR(chunk.pad1)));
	node.AddChild(xml::node("pad2", STR(chunk.pad2)));
	return node;
}

template<>
xml::node CConverter::From(const rw::core::frame_list& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("frame_list");
	node.AddChild(From<rw::core::frame_list_data, xml::node>(chunk.data));
	
	for (auto& extension : chunk)
	{
		node.AddChild(From<rw::core::frame_list_ext, xml::node>(*extension));
	}

	return node;
}

template<>
xml::node CConverter::From(const rw::core::material_list& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("material_list");

	return node;
}

template<>
xml::node CConverter::From(const rw::core::geometry_data& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("geometry_data");

	auto header_node = xml::node("header");
	header_node.AddChild(MAKE_NODE("flags", chunk.header.flags));
	header_node.AddChild(MAKE_NODE("uv_count", chunk.header.uv_count));
	header_node.AddChild(MAKE_NODE("native_flags", chunk.header.native_flags));
	header_node.AddChild(MAKE_NODE("face_count", chunk.header.face_count));
	header_node.AddChild(MAKE_NODE("vertex_count", chunk.header.vertex_count));
	header_node.AddChild(MAKE_NODE("frame_count", chunk.header.frame_count));
	node.AddChild(header_node);

	if (chunk.version == 4099)
	{
		auto lighting_node = xml::node("lighting");
		lighting_node.AddChild(MAKE_NODE("ambient", chunk.lighting.ambient));
		lighting_node.AddChild(MAKE_NODE("diffuse", chunk.lighting.diffuse));
		lighting_node.AddChild(MAKE_NODE("specular", chunk.lighting.specular));
		node.AddChild(lighting_node);
	}

	if (chunk.header.flags & rw::rpGEOMETRYPRELIT)
	{
		auto colours_node = xml::node("colours");
		for (int i = 0; i < chunk.header.vertex_count; i++)
		{
			auto colour_node = xml::node("colour");
			colour_node["_i"] = STR(i);
			colour_node["red"] = STR(chunk.colours[i].red);
			colour_node["green"] = STR(chunk.colours[i].green);
			colour_node["blue"] = STR(chunk.colours[i].blue);
			colour_node["alpha"] = STR(chunk.colours[i].alpha);
			colours_node.AddChild(colour_node);
		}
		node.AddChild(colours_node);
	}

	if (chunk.header.flags & rw::rpGEOMETRYTEXTURED)
	{
		auto texture_mappings_node = xml::node("texture_mappings");
		for (int i = 0; i < chunk.header.vertex_count; i++)
		{
			auto texture_mapping_node = xml::node("texture_mapping");
			texture_mapping_node["_i"] = STR(i);
			texture_mapping_node["u"] = STR(chunk.texture_mappings[i].u);
			texture_mapping_node["v"] = STR(chunk.texture_mappings[i].v);
			texture_mappings_node.AddChild(texture_mapping_node);
		}
		node.AddChild(texture_mappings_node);
	}

	auto faces_node = xml::node("faces");
	for (int i = 0; i < chunk.header.face_count; i++)
	{
		auto face_node = xml::node("face");
		face_node["_i"] = STR(i);
		face_node["v1"] = STR(chunk.faces[i].v1);
		face_node["v2"] = STR(chunk.faces[i].v2);
		face_node["v3"] = STR(chunk.faces[i].v3);
		face_node["flags"] = STR(chunk.faces[i].flags);
		faces_node.AddChild(face_node);
	}
	node.AddChild(faces_node);

	auto bounding_sphere_node = xml::node("bounding_sphere");
	bounding_sphere_node.AddChild(MAKE_NODE("pos.x", (float)chunk.bounding_sphere.pos.x));
	bounding_sphere_node.AddChild(MAKE_NODE("pos.y", (float)chunk.bounding_sphere.pos.y));
	bounding_sphere_node.AddChild(MAKE_NODE("pos.z", (float)chunk.bounding_sphere.pos.z));
	bounding_sphere_node.AddChild(MAKE_NODE("radius", (float)chunk.bounding_sphere.radius));
	node.AddChild(bounding_sphere_node);

	auto flags_node = xml::node("flags");
	flags_node.AddChild(MAKE_NODE("has_position", chunk.flags.has_position));
	flags_node.AddChild(MAKE_NODE("has_normals", chunk.flags.has_normals));
	node.AddChild(flags_node);

	auto vertices_node = xml::node("vertices");
	for (int i = 0; i < chunk.header.vertex_count; i++)
	{
		auto vertex_node = xml::node("vertex");
		vertex_node["_i"] = STR(i);
		vertex_node["x"] = STR(chunk.vertices[i].x);
		vertex_node["y"] = STR(chunk.vertices[i].y);
		vertex_node["z"] = STR(chunk.vertices[i].z);
		vertices_node.AddChild(vertex_node);
	}
	node.AddChild(vertices_node);

	if (chunk.header.flags & rw::rpGEOMETRYNORMALS)
	{
		auto normals_node = xml::node("normals");
		for (int i = 0; i < chunk.header.vertex_count; i++)
		{
			auto normal_node = xml::node("normal");
			normal_node["_i"] = STR(i);
			normal_node["x"] = STR(chunk.normals[i].x);
			normal_node["y"] = STR(chunk.normals[i].y);
			normal_node["z"] = STR(chunk.normals[i].z);
			normals_node.AddChild(normal_node);
		}
		node.AddChild(normals_node);
	}

	return node;
}

template<>
xml::node CConverter::From(const rw::core::geometry_ext& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("geometry_ext");

	return node;
}

template<>
xml::node CConverter::From(const rw::core::geometry& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("geometry");
	node.AddChild(CConverter::From<rw::core::geometry_data, xml::node>(chunk.data));
	node.AddChild(CConverter::From<rw::core::material_list, xml::node>(chunk.materials));
	node.AddChild(CConverter::From<rw::core::geometry_ext, xml::node>(chunk.extension));
	return node;
}

template<>
xml::node CConverter::From(const rw::core::geometry_list_data& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("geometry_list_data");
	node.AddChild(xml::node("geometry_count", STR(chunk.geometry_count)));
	return node;
}

template<>
xml::node CConverter::From(const rw::core::geometry_list& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("geometry_list");
	node.AddChild(From<rw::core::geometry_list_data, xml::node>(chunk.data));
	
	for (auto& geometry : chunk)
	{
		node.AddChild(From<rw::core::geometry, xml::node>(*geometry));
	}
	return node;
}

template<>
xml::node CConverter::From(const rw::core::clump& chunk)
{
	auto node = From<rw::chunk_base, xml::node>(chunk);
	node.SetName("clump");
	node.AddChild(From<rw::core::clump_data, xml::node>(chunk.data));
	node.AddChild(From<rw::core::frame_list, xml::node>(chunk.frames));
	node.AddChild(From<rw::core::geometry_list, xml::node>(chunk.geometries));
	node.UpdateIndentsForSubnodes();
	return node;
}