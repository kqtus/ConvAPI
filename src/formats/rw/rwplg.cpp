#include "../formats_pch.h"
#include "rwplg.h"

bool rw::plg::bin_mesh::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, flags);
	READ_VAR(stream, mesh_count);
	READ_VAR(stream, total_indices);

	INIT_ARR(meshes, mesh_count);
	for (int i = 0; i < mesh_count; i++)
	{
		READ_VAR(stream, meshes[i].index_count);
		READ_VAR(stream, meshes[i].mat_index);

		INIT_ARR(meshes[i].indices, meshes[i].index_count);
		for (int j = 0; j < meshes[i].index_count; j++)
		{
			READ_VAR(stream, meshes[i].indices[j]);
		}
	}

	return true;
}

bool rw::plg::morph::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, morph_target_index);
	return true;
}

bool rw::plg::sky_mipmap_val::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, kl_value);
	return true;
}

bool rw::plg::frame::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	
	INIT_ARR(name, size);
	for (int i = 0; i < size; i++)
	{
		READ_VAR(stream, name[i]);
	}

	return true;
}

bool rw::plg::hanim::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);

	// #TODO Read it, don't skip it
	uint8_t* buffer;
	INIT_ARR(buffer, size);

	for (int i = 0; i < size; i++)
	{
		READ_VAR(stream, buffer[i]);
	}
	delete[] buffer;

	return true;
}
