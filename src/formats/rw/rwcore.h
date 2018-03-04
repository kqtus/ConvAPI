#pragma once
#include <vector>
#include "../../utilities/maths.h"

#include "rwplg.h"

namespace rw
{
	namespace core
	{
		class extension 
			: public chunk_base
			, public std::vector<chunk_base*>
		{
			CONVERTIBLE_ENTITY
		public:
			extension();
			extension(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class texture_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			uint16_t filter_mode_flags;
			uint16_t pad;

		public:
			texture_data();
			texture_data(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class texture : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			texture_data data;
			string texture_name;
			string mask_name;
			extension ext;

		public:
			texture();
			texture(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class material_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			uint32_t pad1;
			rgba color;
			uint32_t pad2;
			uint32_t texture_count;
			vec3<float_t> unk_vec;

		public:
			material_data();
			material_data(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class material : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			material_data data;
			texture** textures;
			extension ext;

		public:
			material();
			material(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class material_list_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			uint32_t material_count;
			uint32_t* material_indices;

		public:
			material_list_data();
			material_list_data(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class material_list 
			: public chunk_base
			, private std::vector<material*>
		{
			CONVERTIBLE_ENTITY
		protected:
			material_list_data data;

		public:
			material_list();
			material_list(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;

			using vector::operator[];
		};

		class geometry_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			struct
			{
				uint32_t flags;
				uint32_t face_count;
				uint32_t vertex_count;
				uint32_t frame_count;
			} header;

			struct
			{
				uint32_t ambient;
				uint32_t diffuse;
				uint32_t specular;
			} lighting;

			struct colour
			{
				uint8_t red;
				uint8_t green;
				uint8_t blue;
				uint8_t alpha;
			};

			struct texture_mapping
			{
				float_t u;
				float_t v;
			};

			struct face
			{
				uint16_t v2;
				uint16_t v1;
				uint16_t flags;
				uint16_t v3;
			};

			colour* colours;
			texture_mapping* texture_mappings;
			texture_mapping* texture_mappings2;
			face* faces;

			struct
			{
				vec3<float_t> pos;
				float_t radius;
			} bounding_sphere;

			struct
			{
				uint32_t has_position;
				uint32_t has_normals;
			} flags;

			vec3<float_t>* vertices;
			vec3<float_t>* normals;

		public:
			geometry_data();
			geometry_data(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class geometry : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			geometry_data data;
			material_list materials;
			extension ext;

		public:
			geometry();
			geometry(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class geometry_list_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			uint32_t geometry_count;

		public:
			geometry_list_data();
			geometry_list_data(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class geometry_list 
			: public chunk_base
			, private std::vector<geometry*>
		{
			CONVERTIBLE_ENTITY
		protected:
			geometry_list_data data;

		public:
			geometry_list();
			geometry_list(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;

			using vector::operator[];
		};

		class frame_list_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			uint32_t frame_count;

			struct
			{
				mat3x3<float_t> rotation;
				vec3<float_t> coords_offset;
				uint32_t parent_frame;
				uint32_t pad;
			}* infos;

		public:
			frame_list_data();
			frame_list_data(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class frame_list 
			: public chunk_base
			, private std::vector<extension*>
		{
			CONVERTIBLE_ENTITY
		protected:
			frame_list_data data;

		public:
			frame_list();
			frame_list(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;

			using vector::operator[];
		};

		class atomic_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			uint32_t frame_index;
			uint32_t geometry_index;
			uint32_t flags;
			uint32_t pad;

		public:
			atomic_data();
			atomic_data(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class atomic : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			atomic_data data;
			// geometry geom; - might have geometry if clump's geometry list is empty
			extension ext;

		public:
			atomic();
			atomic(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class clump_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			uint32_t object_count;
			uint32_t light_count;
			uint32_t camera_count;

		public:
			clump_data();
			clump_data(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class clump : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			clump_data data;
			frame_list frames;
			geometry_list geometries;
			std::vector<atomic> atomics;
			extension ext;

		public:
			clump();
			clump(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};
	}

	namespace core
	{
		class texture_native_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			uint32_t platform_id;
			uint8_t filter_mode;
			uint8_t uv_addressing;
			uint8_t* name;
			uint8_t* mask_name;
			uint16_t pad;

			uint32_t raster_format;

			union
			{
				uint32_t has_alpha;
				uint32_t d3d_format;
			};

			uint16_t width;
			uint16_t height;
			uint8_t depth;
			uint8_t level_count;
			uint8_t raster_type;

			union
			{
				uint8_t compression;

				struct
				{
					uint8_t alpha : 1;
					uint8_t cube_texture : 1;
					uint8_t auto_mip_maps : 1;
					uint8_t compressed : 1;
					uint8_t pad2 : 4;
				};
			};

		public:
			texture_native_data();
			texture_native_data(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class texture_native : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			texture_native_data data;
			extension ext;

		public:
			texture_native();
			texture_native(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class texture_dictionary_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			union
			{
				struct
				{
					uint32_t texture_count;
				} old_version;

				struct
				{
					uint16_t texture_count;
					uint16_t device_id;
				} new_version;
			};

		public:
			texture_dictionary_data();
			texture_dictionary_data(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class texture_dictionary 
			: public chunk_base
			, public std::vector<texture_native*>
		{
			CONVERTIBLE_ENTITY
		protected:
			texture_dictionary_data data;
			extension ext;

		public:
			texture_dictionary();
			texture_dictionary(uint32_t type);

			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};
	}
}