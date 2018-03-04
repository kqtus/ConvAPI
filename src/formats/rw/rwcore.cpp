#include "rwcore.h"
#include <type_traits>

rw::core::extension::extension()
	: extension(DEFAULT_RW_TYPE)
{
}

rw::core::extension::extension(uint32_t type)
	: chunk_base(rwID_EXTENSION, type)
{
}

rw::core::texture_data::texture_data()
	: texture_data(DEFAULT_RW_TYPE)
{
}

rw::core::texture_data::texture_data(uint32_t type)
	: chunk_base(rwID_STRUCT, type)
	, filter_mode_flags(0)
	, pad(0)
{
}

rw::core::texture::texture()
	: texture(DEFAULT_RW_TYPE)
{
}

rw::core::texture::texture(uint32_t type)
	: chunk_base(rwID_TEXTURE, type) // OR TEXTURENATIVE ?
	, data(type)
	, texture_name(type)
	, mask_name(type)
	, ext(type)
{
}

rw::core::material_data::material_data()
	: material_data(DEFAULT_RW_TYPE)
{
}

rw::core::material_data::material_data(uint32_t type)
	: chunk_base(rwID_STRUCT, type)
	, pad1(0)
	, pad2(0)
	, texture_count(0)
{
}

rw::core::material::material()
	: material(DEFAULT_RW_TYPE)
{
}

rw::core::material::material(uint32_t type)
	: chunk_base(rwID_MATERIAL, type)
	, data(type)
	, textures(nullptr)
	, ext(type)
{
}

rw::core::material_list_data::material_list_data()
	: material_list_data(DEFAULT_RW_TYPE)
{
}

rw::core::material_list_data::material_list_data(uint32_t type)
	: chunk_base(rwID_STRUCT, type)
	, material_count(0)
	, material_indices(nullptr)
{
}

rw::core::material_list::material_list()
	: material_list(DEFAULT_RW_TYPE)
{
}

rw::core::material_list::material_list(uint32_t type)
	: chunk_base(rwID_MATLIST, type)
	, data(type)
{
}

rw::core::geometry_data::geometry_data()
	: geometry_data(DEFAULT_RW_TYPE)
{
}

rw::core::geometry_data::geometry_data(uint32_t type)
	: chunk_base(rwID_STRUCT, type)
	, colours(nullptr)
	, texture_mappings(nullptr)
	, faces(nullptr)
	, vertices(nullptr)
	, normals(nullptr)
{
}

rw::core::geometry::geometry()
	: geometry(DEFAULT_RW_TYPE)
{
}

rw::core::geometry::geometry(uint32_t type)
	: chunk_base(rwID_GEOMETRY, type)
	, data(type)
	, materials(type)
	, ext(type)
{
}

rw::core::geometry_list_data::geometry_list_data()
	: geometry_list_data(DEFAULT_RW_TYPE)
{
}

rw::core::geometry_list_data::geometry_list_data(uint32_t type)
	: chunk_base(rwID_STRUCT, type)
	, geometry_count(0)
{
}

rw::core::geometry_list::geometry_list()
	: geometry_list(DEFAULT_RW_TYPE)
{
}

rw::core::geometry_list::geometry_list(uint32_t type)
	: chunk_base(rwID_GEOMETRYLIST, type)
	, data(type)
{
}

rw::core::frame_list_data::frame_list_data()
	: frame_list_data(DEFAULT_RW_TYPE)
{
}

rw::core::frame_list_data::frame_list_data(uint32_t type)
	: chunk_base(rwID_STRUCT, type)
	, frame_count(0)
	, infos(nullptr)
{
}

rw::core::frame_list::frame_list()
	: frame_list(DEFAULT_RW_TYPE)
{
}

rw::core::frame_list::frame_list(uint32_t type)
	: chunk_base(rwID_FRAMELIST, type)
	, data(type)
{
}

rw::core::atomic_data::atomic_data()
	: atomic_data(DEFAULT_RW_TYPE)
{
}

rw::core::atomic_data::atomic_data(uint32_t type)
	: chunk_base(rwID_STRUCT, type)
	, frame_index(0)
	, geometry_index(0)
	, flags(0)
	, pad(0)
{
}

rw::core::atomic::atomic()
	: atomic(DEFAULT_RW_TYPE)
{
}

rw::core::atomic::atomic(uint32_t type)
	: chunk_base(rwID_ATOMIC, type)
	, data(type)
	, ext(type)
{
}

rw::core::clump_data::clump_data()
	: clump_data(DEFAULT_RW_TYPE)
{
}

rw::core::clump_data::clump_data(uint32_t type)
	: chunk_base(rwID_STRUCT, type)
	, object_count(0)
	, light_count(0)
	, camera_count(0)
{
}

rw::core::clump::clump()
	: chunk_base(rwID_CLUMP)
{
}

rw::core::clump::clump(uint32_t type)
	: chunk_base(rwID_CLUMP, type)
{
}

bool rw::core::extension::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	size_t extension_beg_offset = stream.Tell();
	size_t extension_end_offset = extension_beg_offset + chunk_base::size;

	while (stream.Tell() < extension_end_offset)
	{
		chunk_base* child = plg::DecodeAndReadPlg(stream);
		this->push_back(child);
	}
	return true;
}

bool rw::core::extension::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	
	for (auto& child : *this)
	{
		child->Write(stream);
	}

	return true;
}

bool rw::core::texture_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, filter_mode_flags);
	READ_VAR(stream, pad);
	return true;
}

bool rw::core::texture_data::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, filter_mode_flags);
	WRITE_VAR(stream, pad);
	return true;
}

bool rw::core::texture::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);
	texture_name.Read(stream);
	mask_name.Read(stream);
	ext.Read(stream);
	return true;
}

bool rw::core::texture::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	data.Write(stream);
	texture_name.Write(stream);
	mask_name.Write(stream);
	ext.Write(stream);
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

bool rw::core::material_data::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, pad1);
	WRITE_VAR(stream, color.r);
	WRITE_VAR(stream, color.g);
	WRITE_VAR(stream, color.b);
	WRITE_VAR(stream, color.a);
	WRITE_VAR(stream, pad2);
	WRITE_VAR(stream, texture_count);
	WRITE_VAR(stream, unk_vec.x);
	WRITE_VAR(stream, unk_vec.y);
	WRITE_VAR(stream, unk_vec.z);
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

	ext.Read(stream);
	return true;
}

bool rw::core::material::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	data.Write(stream);

	for (int i = 0; i < data.texture_count; i++)
	{
		textures[i]->Write(stream);
	}

	ext.Write(stream);
	return true;
}

bool rw::core::material_list_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, material_count);

	INIT_ARR(material_indices, material_count);
	for (int i = 0; i < material_count; i++)
	{
		READ_VAR(stream, material_indices[i]);
	}

	return true;
}

bool rw::core::material_list_data::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, material_count);

	for (int i = 0; i < material_count; i++)
	{
		WRITE_VAR(stream, material_indices[i]);
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

bool rw::core::material_list::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	data.Write(stream);

	for (auto& mat : *this)
	{
		mat->Write(stream);
	}

	return true;
}

bool rw::core::geometry_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, header.flags);
	READ_VAR(stream, header.face_count);
	READ_VAR(stream, header.vertex_count);
	READ_VAR(stream, header.frame_count);

	if (DecodeVersion() < 0x34000)
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

	if ((header.flags & rpGEOMETRYTEXTURED) || (header.flags & rpGEOMETRYTEXTURED2))
	{
		INIT_ARR(texture_mappings, header.vertex_count);
		for (int i = 0; i < header.vertex_count; i++)
		{
			READ_VAR(stream, texture_mappings[i].u);
			READ_VAR(stream, texture_mappings[i].v);
		}
	}

	if (header.flags & rpGEOMETRYTEXTURED2)
	{
		INIT_ARR(texture_mappings2, header.vertex_count);
		for (int i = 0; i < header.vertex_count; i++)
		{
			READ_VAR(stream, texture_mappings2[i].u);
			READ_VAR(stream, texture_mappings2[i].v);
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

bool rw::core::geometry_data::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, header.flags);
	WRITE_VAR(stream, header.face_count);
	WRITE_VAR(stream, header.vertex_count);
	WRITE_VAR(stream, header.frame_count);

	if (DecodeVersion() < 0x34000)
	{
		WRITE_VAR(stream, lighting.ambient);
		WRITE_VAR(stream, lighting.diffuse);
		WRITE_VAR(stream, lighting.specular);
	}

	if (header.flags & rpGEOMETRYPRELIT)
	{
		for (int i = 0; i < header.vertex_count; i++)
		{
			WRITE_VAR(stream, colours[i].red);
			WRITE_VAR(stream, colours[i].green);
			WRITE_VAR(stream, colours[i].blue);
			WRITE_VAR(stream, colours[i].alpha);
		}
	}

	if ((header.flags & rpGEOMETRYTEXTURED) || (header.flags & rpGEOMETRYTEXTURED2))
	{
		for (int i = 0; i < header.vertex_count; i++)
		{
			WRITE_VAR(stream, texture_mappings[i].u);
			WRITE_VAR(stream, texture_mappings[i].v);
		}
	}

	if (header.flags & rpGEOMETRYTEXTURED2)
	{
		for (int i = 0; i < header.vertex_count; i++)
		{
			WRITE_VAR(stream, texture_mappings2[i].u);
			WRITE_VAR(stream, texture_mappings2[i].v);
		}
	}
	for (int i = 0; i < header.face_count; i++)
	{
		WRITE_VAR(stream, faces[i].v2);
		WRITE_VAR(stream, faces[i].v1);
		WRITE_VAR(stream, faces[i].flags);
		WRITE_VAR(stream, faces[i].v3);
	}

	WRITE_VAR(stream, bounding_sphere.pos.x);
	WRITE_VAR(stream, bounding_sphere.pos.y);
	WRITE_VAR(stream, bounding_sphere.pos.z);
	WRITE_VAR(stream, bounding_sphere.radius);

	WRITE_VAR(stream, flags.has_position);
	WRITE_VAR(stream, flags.has_normals);

	for (int i = 0; i < header.vertex_count; i++)
	{
		WRITE_VAR(stream, vertices[i].x);
		WRITE_VAR(stream, vertices[i].y);
		WRITE_VAR(stream, vertices[i].z);
	}

	if (header.flags & rpGEOMETRYNORMALS)
	{
		for (int i = 0; i < header.vertex_count; i++)
		{
			WRITE_VAR(stream, normals[i].x);
			WRITE_VAR(stream, normals[i].y);
			WRITE_VAR(stream, normals[i].z);
		}
	}

	return true;
}

bool rw::core::geometry::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);
	materials.Read(stream);
	ext.Read(stream);
	return true;
}

bool rw::core::geometry::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	data.Write(stream);
	materials.Write(stream);
	ext.Write(stream);
	return true;
}

bool rw::core::geometry_list_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, geometry_count);
	return true;
}

bool rw::core::geometry_list_data::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, geometry_count);
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

bool rw::core::geometry_list::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	data.Write(stream);

	for (auto& geom : *this)
	{
		geom->Write(stream);
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

bool rw::core::frame_list_data::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, frame_count);

	for (int i = 0; i < frame_count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				WRITE_VAR(stream, infos[i].rotation[j][k]);
			}
		}

		WRITE_VAR(stream, infos[i].coords_offset.x);
		WRITE_VAR(stream, infos[i].coords_offset.y);
		WRITE_VAR(stream, infos[i].coords_offset.z);
		WRITE_VAR(stream, infos[i].parent_frame);
		WRITE_VAR(stream, infos[i].pad);
	}

	return true;
}

bool rw::core::frame_list::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);

	this->reserve(data.frame_count);
	for (int i = 0; i < data.frame_count; i++)
	{
		auto ext = new extension();
		ext->Read(stream);
		this->push_back(ext);
	}

	return true;
}

bool rw::core::frame_list::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	data.Write(stream);

	for (auto& ext : *this)
	{
		ext->Write(stream);
	}

	return true;
}

bool rw::core::atomic_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, frame_index);
	READ_VAR(stream, geometry_index);
	READ_VAR(stream, flags);
	READ_VAR(stream, pad);
	return true;
}

bool rw::core::atomic_data::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, frame_index);
	WRITE_VAR(stream, geometry_index);
	WRITE_VAR(stream, flags);
	WRITE_VAR(stream, pad);
	return true;
}

bool rw::core::atomic::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);
	ext.Read(stream);
	return true;
}

bool rw::core::atomic::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	data.Write(stream);
	ext.Write(stream);
	return true;
}

bool rw::core::clump_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, object_count);
	READ_VAR(stream, light_count);
	READ_VAR(stream, camera_count);
	return true;
}

bool rw::core::clump_data::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, object_count);
	WRITE_VAR(stream, light_count);
	WRITE_VAR(stream, camera_count);
	return true;
}

bool rw::core::clump::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);
	frames.Read(stream);
	geometries.Read(stream);
	
	for (int i = 0; i < data.object_count; i++)
	{
		atomic cur_atomic;
		cur_atomic.Read(stream);
		atomics.push_back(cur_atomic);
	}
	// #TODO: Read lights and cameras
	// now it is assumed that model doesn't have them
	ext.Read(stream);
	return true;
}

bool rw::core::clump::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	data.Write(stream);
	frames.Write(stream);
	geometries.Write(stream);

	for (auto& cur_atomic : atomics)
	{
		cur_atomic.Write(stream);
	}

	ext.Write(stream);
	return true;
}

rw::core::texture_native_data::texture_native_data()
	: texture_native_data(DEFAULT_RW_TYPE)
{
}

rw::core::texture_native_data::texture_native_data(uint32_t type)
	: chunk_base(rwID_STRUCT, type)
	, platform_id(0)
	, filter_mode(0)
	, uv_addressing(0)
	, name(nullptr)
	, mask_name(nullptr)
	, pad(0)
	, raster_format(0)
	, has_alpha(0)
	, width(0)
	, height(0)
	, depth(0)
	, level_count(0)
	, raster_type(0)
	, compression(0)
{
}

rw::core::texture_native::texture_native()
	: texture_native(DEFAULT_RW_TYPE)
{
}

rw::core::texture_native::texture_native(uint32_t type)
	: chunk_base(rwID_TEXTURENATIVE, type)
	, data(type)
	, ext(type)
{
}

rw::core::texture_dictionary_data::texture_dictionary_data()
	: texture_dictionary_data(DEFAULT_RW_TYPE)
{
}

rw::core::texture_dictionary_data::texture_dictionary_data(uint32_t type)
	: chunk_base(rwID_STRUCT, type)
{
}

rw::core::texture_dictionary::texture_dictionary()
	: texture_dictionary(DEFAULT_RW_TYPE)
{
}

rw::core::texture_dictionary::texture_dictionary(uint32_t type)
	: chunk_base(rwID_TEXDICTIONARY, type)
	, data(type)
	, ext(type)
{
}

bool rw::core::texture_native_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, platform_id);
	READ_VAR(stream, filter_mode);
	READ_VAR(stream, uv_addressing);
	READ_ARR(stream, name, 32);
	READ_ARR(stream, mask_name, 32);
	READ_VAR(stream, pad);
	READ_VAR(stream, raster_format);
	READ_VAR(stream, has_alpha);
	READ_VAR(stream, width);
	READ_VAR(stream, height);
	READ_VAR(stream, depth);
	READ_VAR(stream, level_count);
	READ_VAR(stream, raster_type);
	READ_VAR(stream, compression);

	// #TODO: Read mimaps and actual image data

	return true;
}

bool rw::core::texture_native_data::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, platform_id);
	WRITE_VAR(stream, filter_mode);
	WRITE_VAR(stream, uv_addressing);
	WRITE_ARR(stream, name, 32);
	WRITE_ARR(stream, mask_name, 32);
	WRITE_VAR(stream, pad);
	WRITE_VAR(stream, raster_format);
	WRITE_VAR(stream, has_alpha);
	WRITE_VAR(stream, width);
	WRITE_VAR(stream, height);
	WRITE_VAR(stream, depth);
	WRITE_VAR(stream, level_count);
	WRITE_VAR(stream, raster_type);
	WRITE_VAR(stream, compression);

	// #TODO: Write mimaps and actual image data

	return true;
}

bool rw::core::texture_native::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);
	ext.Read(stream);
	return true;
}

bool rw::core::texture_native::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	data.Write(stream);
	ext.Write(stream);
	return true;
}

bool rw::core::texture_dictionary_data::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, old_version.texture_count);
	return true;
}

bool rw::core::texture_dictionary_data::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, old_version.texture_count);
	return true;
}

bool rw::core::texture_dictionary::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	data.Read(stream);

	// #TODO: Add switch to manage old/new version
	for (int i = 0; i < data.old_version.texture_count; i++)
	{
		auto tex_native = new texture_native();
		tex_native->Read(stream);
		this->push_back(tex_native);
	}

	ext.Read(stream);
	return true;
}

bool rw::core::texture_dictionary::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	data.Write(stream);

	for (auto& tex_native : *this)
	{
		tex_native->Write(stream);
	}

	ext.Write(stream);
	return true;
}