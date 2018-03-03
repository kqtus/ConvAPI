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

bool rw::plg::bin_mesh::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, flags);
	WRITE_VAR(stream, mesh_count);
	WRITE_VAR(stream, total_indices);

	for (int i = 0; i < mesh_count; i++)
	{
		WRITE_VAR(stream, meshes[i].index_count);
		WRITE_VAR(stream, meshes[i].mat_index);

		for (int j = 0; j < meshes[i].index_count; j++)
		{
			WRITE_VAR(stream, meshes[i].indices[j]);
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

bool rw::plg::morph::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, morph_target_index);
	return true;
}

bool rw::plg::sky_mipmap_val::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	READ_VAR(stream, kl_value);
	return true;
}

bool rw::plg::sky_mipmap_val::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);
	WRITE_VAR(stream, kl_value);
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

bool rw::plg::frame::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);

	for (int i = 0; i < size; i++)
	{
		WRITE_VAR(stream, name[i]);
	}

	return true;
}

bool rw::plg::hanim::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);

	// #TODO Read it, don't skip it

	return true;
}

bool rw::plg::hanim::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);

	// #TODO: Write proper data when reading implementation is done.
	return true;
}

bool rw::plg::unknown::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);
	INIT_ARR(data, size);

	for (int i = 0; i < size; i++)
	{
		READ_VAR(stream, data[i]);
	}

	return true;
}

bool rw::plg::unknown::Write(out_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Write(stream);

	for (int i = 0; i < size; i++)
	{
		WRITE_VAR(stream, data[i]);
	}

	return true;
}

#define CREATE_OBJ_IF_EQUAL(obj_inst, lhs_cond, rhs_cond, T) \
if (lhs_cond == rhs_cond) \
	obj_inst = new T();

rw::chunk_base* rw::plg::DecodeAndReadPlg(in_stream<EStreamType::BINARY>& stream)
{
	size_t plg_offset = stream.Tell();
	uint32_t plg_type;
	chunk_base* ret_plg = nullptr;;

	READ_VAR(stream, plg_type);
	stream.Seek(plg_offset, 0);

	CREATE_OBJ_IF_EQUAL(ret_plg, plg_type, rwID_MORPHPLG,			plg::morph);
	CREATE_OBJ_IF_EQUAL(ret_plg, plg_type, rwID_BINMESHPLG,			plg::bin_mesh);
	CREATE_OBJ_IF_EQUAL(ret_plg, plg_type, rwID_SKYMIPMAPVALPLG,	plg::sky_mipmap_val);
	CREATE_OBJ_IF_EQUAL(ret_plg, plg_type, rwID_FRAMEPLG,			plg::frame);
	//CREATE_OBJ_IF_EQUAL(ret_plg, plg_type, rwID_HANIMPLG,			plg::hanim);

	if (ret_plg == nullptr)
	{
		ret_plg = new unknown();
	}

	ret_plg->Read(stream);
	return ret_plg;
}