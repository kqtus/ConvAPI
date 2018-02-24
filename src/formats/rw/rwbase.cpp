#include "rwbase.h"

rw::chunk_base::chunk_base()
	: type(0)
	, size(0)
	, version(0)
{
}

bool rw::chunk_base::Read(in_stream<EStreamType::BINARY>& stream)
{
	READ_VAR(stream, type);
	READ_VAR(stream, size);
	READ_VAR(stream, version);
	return true;
}

bool rw::string::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);

	this->resize(chunk_base::size);
	for (int i = 0; i < chunk_base::size; i++)
	{
		READ_VAR(stream, this->operator[](i));
	}

	return true;
}

std::string rw::chunk_base::ToString() const
{
	return "chunk_base:\n<section_type>: " + std::to_string(type) +
		"\n<section_size>: " + std::to_string(size) +
		"\n<version>: " + std::to_string(version);
}

