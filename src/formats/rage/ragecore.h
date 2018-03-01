#pragma once
#include "ragebase.h"

namespace rage
{
	namespace core
	{
		enum class EShaderEffectParamType : uint8_t
		{
			TEXTURE = 0,
			VECTOR4 = 1,
			MATRIX = 4,
			VECTOR4x6 = 8,
			VECTOR4x14 = 14,
			VECTOR4x15 = 15,
			VECTOR4x16 = 16
		};

		class texture : public block_base
		{
		protected:
			uint8_t object_type;
			uint8_t depth;
			uint16_t usage_count;
			uint32_t pad1;
			uint32_t pad2;
			ptr<uint8_t> name;
			uint32_t native_handle;
			uint16_t width;
			uint16_t height;
			uint32_t pixel_format;
			uint16_t stride;
			uint8_t texture_type;
			uint8_t levels;
		};

		class texture_pc : public texture
		{
		protected:
			float_t pad1[3]; // always 28?
			float_t pad2[3]; // always 34?
			uint32_t next;
			uint32_t prev;
			ptr<void> pixel_data;
			uint8_t pad3[4];
		};

		class shader_effect : public common::IBinReadable
		{
		protected:
			ptr<ptr<void>> parameters;
			ptr<int> null;
			uint32_t parameter_count;
			uint32_t parameter_data_size;
			ptr<EShaderEffectParamType> parameter_types;
			uint32_t shader_hash;
			uint32_t unk1;
			uint32_t unk2;
			ptr<uint32_t> parameter_name_hashes;
			uint32_t unk3;
			uint32_t unk4;
			uint32_t unk5;
		};

		class shader : public block_base
		{
		protected:
			uint8_t version;
			uint8_t draw_bucket;
			uint8_t usage_count;
			uint8_t pad;
			uint16_t unk1;
			uint16_t shader_index;
			uint32_t unk2;
			shader_effect effect;
		};

		class shader_fx : public shader
		{
		protected:
			ptr<char> shader_name;
			ptr<char> sps_name;
			uint32_t unk1;
			uint32_t unk2;
			uint32_t preset;
			uint32_t unk3;
		};

		class shader_group // : public datBase
		{
		protected:
			ptr<dict<texture_pc>> textures;
			obj_arr<shader_fx> shaders;
			obj_arr<uint32_t> unks1;
			obj_arr<uint32_t> unks2;
			obj_arr<uint32_t> unks3;
			ptr<int> unk1;
			ptr<int> unk2;
			obj_arr<uint32_t> unks4;
			arr<uint32_t> vertex_formats;
			arr<uint32_t> shader_indices;
		};

		class drawable_base : public block_base
		{
		protected:
			ptr<shader_group> shader_grp;
		};

		class index_buffer // : public datBase
		{
		protected:
			uint32_t index_count;
			ptr<uint16_t> index_data; // with PHYSICAL_VERTICES ?
		};

		class index_buffer_d3d : public index_buffer
		{
		protected:
			void* index_buf;
			uint32_t unks[8];
		};

		class vertex_format // : public pgStreamableBase
		{
		protected:
			uint32_t mask;
			uint8_t vertex_size;
			uint8_t unk1;
			uint8_t unk2;
			uint8_t vertex_field_count;
			uint64_t vertex_fields;
		};

		class vertex_buffer // : public datBase
		{
		protected:
			uint16_t vertex_count;
			uint8_t locked;
			uint8_t pad;
			ptr<void> locked_data;
			uint32_t vertex_size;
			ptr<vertex_format> vertex_format;
			uint32_t lock_thread_id;
			ptr<void> vertex_data; // with 'true' in template
		};

		class vertex_buffer_d3d : public vertex_buffer
		{
		protected:
			void* vertex_buf;
			uint32_t unks[8];
		};

		class geometry_qb // : public datBase
		{
		protected:
			ptr<void> vertex_declaration;
			uint32_t object_type;
			ptr<vertex_buffer_d3d> vertex_buffers[4];
			ptr<index_buffer_d3d> index_buffers[4];
			uint32_t index_count;
			uint32_t face_count;
			uint16_t vertex_count;
			uint16_t indices_per_face;
			ptr<uint16_t> bone_mapping;
			uint16_t vertex_stride;
			uint16_t bone_count;
			ptr<void> vertex_declaration_instance;
			ptr<void> vertex_buffer_instance;
			uint32_t use_global_stream_index;
		};

		class model // : public datBase
		{
		protected:
			object_array<geometry_qb> geometries;
			ptr<vec4> geometry_bounds;
			ptr<uint16_t> shader_mappings;
			uint8_t bone_count;
			uint8_t skinned;
			uint8_t pad1;
			uint8_t pad2;
			uint8_t pad3;
			uint8_t has_bone_mapping;
			uint16_t shader_mapping_count;
		};

		class lod_group
		{
		protected:
			vec4 center;
			vec3 bounds_min;
			vec3 bounds_max;
			ptr<obj_arr<model>> models[4];
			float_t unks[4];
			uint32_t draw_bucket_mask[4];
			float_t radius;
			float_t pad[3];
		};

		class drawable : public drawable_base
		{
		protected:
			ptr<skeleton_data> skeleton_dat;
			lod_group lods;
		};
	}
}