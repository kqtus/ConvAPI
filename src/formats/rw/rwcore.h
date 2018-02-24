#pragma once
#include <vector>
#include "../../utilities/maths.h"

#include "rwplg.h"

namespace rw
{
	namespace core
	{

		class texture_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			uint16_t filter_mode_flags;
			uint16_t pad;

		public:
			bool Read(in_stream& stream) override;
		};

		class texture_ext : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			plg::sky_mipmap_val sky_mimpap_val_plg;

		public:
			bool Read(in_stream& stream) override;
		};

		class texture : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			texture_data data;
			string texture_name;
			string mask_name;
			texture_ext extension;

		public:
			bool Read(in_stream& stream) override;
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
			//material_data();

			bool Read(in_stream& stream) override;
		};

		class material_ext : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			bool Read(in_stream& stream) override;
		};

		class material : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			material_data data;
			texture** textures;
			material_ext extension;

		public:
			//material();

			bool Read(in_stream& stream) override;
		};

		class material_list_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			uint32_t material_count;
			uint32_t pad[11];

		public:
			material_list_data();

			bool Read(in_stream& stream) override;
		};

		class material_list : public chunk_base, private std::vector<material*>
		{
			CONVERTIBLE_ENTITY
		protected:
			material_list_data data;

		public:
			material_list();

			bool Read(in_stream& stream) override;
			using vector::operator[];
		};

		class geometry_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			struct
			{
				uint16_t flags;
				uint8_t uv_count;
				uint8_t native_flags;
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

			bool Read(in_stream& stream) override;
		};

		class geometry_ext : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			plg::bin_mesh bin_mesh_plg;
			plg::morph morph_plg;

		public:
			bool Read(in_stream& stream) override;
		};

		class geometry : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			geometry_data data;
			material_list materials;
			geometry_ext extension;

		public:
			geometry();

			bool Read(in_stream& stream) override;
		};

		class geometry_list_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			uint32_t geometry_count;

		public:
			geometry_list_data();

			bool Read(in_stream& stream) override;
		};

		class geometry_list : public chunk_base, private std::vector<geometry*>
		{
			CONVERTIBLE_ENTITY
		protected:
			geometry_list_data data;

		public:
			geometry_list();

			bool Read(in_stream& stream) override;
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

			bool Read(in_stream& stream) override;
		};

		class frame_list_ext : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			plg::hanim hanim_plg;
			plg::frame frame_plg;

		public:
			bool Read(in_stream& stream) override;
		};

		class frame_list : public chunk_base, private std::vector<frame_list_ext*>
		{
			CONVERTIBLE_ENTITY
		protected:
			frame_list_data data;

		public:
			frame_list();

			bool Read(in_stream& stream) override;
			using vector::operator[];
		};

		class clump_data : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			uint32_t object_count;
			uint32_t pad1;
			uint32_t pad2;

		public:
			clump_data();

			bool Read(in_stream& stream) override;
			//std::string ToString() const;
		};

		class clump : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			clump_data data;
			frame_list frames;
			geometry_list geometries;

		public:
			clump();

			bool Read(in_stream& stream) override;
			std::string ToString() const;
		};
	}
}