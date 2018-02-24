#pragma once
#include "stream.h"

namespace common
{
	class IBinReadable
	{
	public:
		virtual bool Read(in_stream<EStreamType::BINARY>& stream) = 0;
	};

	class IBinWriteable
	{
	public:
		virtual bool Write(out_stream<EStreamType::BINARY>& stream) = 0;
	};

	class ITexReadable
	{
	public:
		virtual bool Read(in_stream<EStreamType::TEXT>& stream) = 0;
	};

	class ITexWriteable
	{
	public:
		virtual bool Write(out_stream<EStreamType::TEXT>& stream) = 0;
	};
}
