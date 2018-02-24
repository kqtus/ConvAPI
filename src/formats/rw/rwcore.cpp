#include "rwcore.h"
#include <type_traits>

rw::core::material_list_data::material_list_data()
	: material_count(0)
{
}

rw::core::material_list::material_list()
{
}

rw::core::geometry_data::geometry_data()
{
}

rw::core::geometry::geometry()
{
}

rw::core::geometry_list_data::geometry_list_data()
	: geometry_count(0)
{
}

rw::core::geometry_list::geometry_list()
{
}

rw::core::frame_list_data::frame_list_data()
	: frame_count(0)
{
}

rw::core::frame_list::frame_list()
{
}

rw::core::clump_data::clump_data()
	: object_count(0)
	, pad1(0)
	, pad2(0)
{
}

rw::core::clump::clump()
{
}

bool rw::core::texture_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, filter_mode_flags);
	READ_VAR(stream, pad);
	return true;
}

bool rw::core::texture_ext::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	sky_mimpap_val_plg.Read(stream);
	return true;
}

bool rw::core::texture::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);
	texture_name.Read(stream);
	mask_name.Read(stream);
	extension.Read(stream);
	return true;
}

bool rw::core::material_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, pad1);
	READ_VAR(stream, color.r);
	READ_VAR(stream, color.g);
	READ_VAR(stream, color.b);
	READ_VAR(stream, color.a);
	READ_VAR(stream, pad2);
	READ_VAR(stream, texture_count);
	READ_VAR(stream, unk_vec.x);
	READ_VAR(stream, unk_vec.y);
	READ_VAR(stream, unk_vec.z);
	return true;
}

bool rw::core::material_ext::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	return true;
}

bool rw::core::material::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);

	INIT_ARR(textures, data.texture_count);
	for (int i = 0; i < data.texture_count; i++)
	{
		textures[i] = new texture();
		textures[i]->Read(stream);
	}

	extension.Read(stream);
	return true;
}

bool rw::core::material_list_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, material_count);

	for (int i = 0; i < 11; i++)
	{
		READ_VAR(stream, pad[i]);
	}

	return true;
}

bool rw::core::material_list::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);

	this->reserve(data.material_count);
	for (int i = 0; i < data.material_count; i++)
	{
		auto mat = new material();
		mat->Read(stream);
		this->push_back(mat);
	}
	return true;
}

bool rw::core::geometry_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, header.flags);
	READ_VAR(stream, header.uv_count);
	READ_VAR(stream, header.native_flags);
	READ_VAR(stream, header.face_count);
	READ_VAR(stream, header.vertex_count);
	READ_VAR(stream, header.frame_count);

	if (version == 4099)
	{
		READ_VAR(stream, lighting.ambient);
		READ_VAR(stream, lighting.diffuse);
		READ_VAR(stream, lighting.specular);
	}

	if (header.flags & rpGEOMETRYPRELIT)
	{
		INIT_ARR(colours, header.vertex_count);
		for (int i = 0; i < header.vertex_count; i++)
		{
			READ_VAR(stream, colours[i].red);
			READ_VAR(stream, colours[i].green);
			READ_VAR(stream, colours[i].blue);
			READ_VAR(stream, colours[i].alpha);
		}
	}

	if (header.flags & rpGEOMETRYTEXTURED)
	{
		INIT_ARR(texture_mappings, header.vertex_count);
		for (int i = 0; i < header.vertex_count; i++)
		{
			READ_VAR(stream, texture_mappings[i].u);
			READ_VAR(stream, texture_mappings[i].v);
		}
	}

	INIT_ARR(faces, header.face_count);
	for (int i = 0; i < header.face_count; i++)
	{
		READ_VAR(stream, faces[i].v2);
		READ_VAR(stream, faces[i].v1);
		READ_VAR(stream, faces[i].flags);
		READ_VAR(stream, faces[i].v3);
	}

	READ_VAR(stream, bounding_sphere.pos.x);
	READ_VAR(stream, bounding_sphere.pos.y);
	READ_VAR(stream, bounding_sphere.pos.z);
	READ_VAR(stream, bounding_sphere.radius);

	READ_VAR(stream, flags.has_position);
	READ_VAR(stream, flags.has_normals);

	INIT_ARR(vertices, header.vertex_count);
	for (int i = 0; i < header.vertex_count; i++)
	{
		READ_VAR(stream, vertices[i].x);
		READ_VAR(stream, vertices[i].y);
		READ_VAR(stream, vertices[i].z);
	}

	if (header.flags & rpGEOMETRYNORMALS)
	{
		INIT_ARR(normals, header.vertex_count);
		for (int i = 0; i < header.vertex_count; i++)
		{
			READ_VAR(stream, normals[i].x);
			READ_VAR(stream, normals[i].y);
			READ_VAR(stream, normals[i].z);
		}
	}

	return true;
}

bool rw::core::geometry_ext::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	bin_mesh_plg.Read(stream);
	morph_plg.Read(stream);
	return true;
}

bool rw::core::geometry::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);
	materials.Read(stream);
	extension.Read(stream);
	return true;
}

bool rw::core::geometry_list_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, geometry_count);
	return true;
}

bool rw::core::geometry_list::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);

	this->reserve(data.geometry_count);
	for (int i = 0; i < data.geometry_count; i++)
	{
		auto current_geometry = new geometry();
		current_geometry->Read(stream);
		this->push_back(current_geometry);
	}

	return true;
}

bool rw::core::frame_list_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, frame_count);

	INIT_ARR(infos, frame_count);
	for (int i = 0; i < frame_count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				READ_VAR(stream, infos[i].rotation[j][k]);
			}
		}

		READ_VAR(stream, infos[i].coords_offset.x);
		READ_VAR(stream, infos[i].coords_offset.y);
		READ_VAR(stream, infos[i].coords_offset.z);
		READ_VAR(stream, infos[i].parent_frame);
		READ_VAR(stream, infos[i].pad);
	}

	return true;
}

bool rw::core::frame_list_ext::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	hanim_plg.Read(stream);
	frame_plg.Read(stream);
	return true;
}

bool rw::core::frame_list::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);

	this->reserve(data.frame_count);
	for (int i = 0; i < data.frame_count; i++)
	{
		auto extension = new frame_list_ext();
		extension->Read(stream);
		this->push_back(extension);
	}

	return true;
}

bool rw::core::clump_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, object_count);
	READ_VAR(stream, pad1);
	READ_VAR(stream, pad2);
	return true;
}
 
bool rw::core::clump::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);
	frames.Read(stream);
	geometries.Read(stream);

	// #TODO: Read optional sections

	return true;
}

std::string rw::core::clump::ToString() const
{
	return "clump:\n" + chunk_base::ToString() +
		"\n<object_count>: " + std::to_string(data.object_count);
}