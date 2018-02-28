#pragma once
#include "rwbase.h"

class rw::string;

namespace rw
{
	namespace plg
	{
		class bin_mesh : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			uint32_t flags;
			uint32_t mesh_count;
			uint32_t total_indices;

			struct
			{
				uint32_t index_count;
				uint32_t mat_index;
				uint32_t* indices; // might be uint16_t when preinstanced and on OpenGL
			}* meshes;

		public:
			bool Read(in_stream<EStreamType::BINARY>& stream) override;
		};

		class morph : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			uint32_t morph_target_index;

		public:
			bool Read(in_stream<EStreamType::BINARY>& stream) override;
		};

		class sky_mipmap_val : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			uint32_t kl_value;

		public:
			bool Read(in_stream<EStreamType::BINARY>& stream) override;
		};

		class frame : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			uint8_t* name;

		public:
			bool Read(in_stream<EStreamType::BINARY>& stream) override;
		};

		class hanim : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			bool Read(in_stream<EStreamType::BINARY>& stream) override;
		};

		class unknown : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			uint8_t* data;

		public:
			bool Read(in_stream<EStreamType::BINARY>& stream) override;
		};

		chunk_base* DecodeAndReadPlg(in_stream<EStreamType::BINARY>& stream);
	}
}