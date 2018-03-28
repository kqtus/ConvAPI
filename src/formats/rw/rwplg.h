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
			bin_mesh();
			bin_mesh(uint32_t type);

			void UpdateSize() override;
			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class morph : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			uint32_t morph_target_index;

		public:
			morph();
			morph(uint32_t type);

			void UpdateSize() override;
			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class sky_mipmap_val : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			uint32_t kl_value;

		public:
			sky_mipmap_val();
			sky_mipmap_val(uint32_t type);

			void UpdateSize() override;
			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class frame : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			uint8_t* name;

		public:
			frame();
			frame(uint32_t type);

			void UpdateSize() override;
			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class hanim : public chunk_base
		{
			CONVERTIBLE_ENTITY
		public:
			hanim();
			hanim(uint32_t type);

			void UpdateSize() override;
			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		class unknown : public chunk_base
		{
			CONVERTIBLE_ENTITY
		protected:
			uint8_t* data;

		public:
			unknown();
			unknown(uint32_t type);

			void UpdateSize() override;
			bool Read(in_stream<EStreamType::BINARY>& stream) override;
			bool Write(out_stream<EStreamType::BINARY>& stream) override;
		};

		chunk_base* DecodeAndReadPlg(in_stream<EStreamType::BINARY>& stream);
	}
}