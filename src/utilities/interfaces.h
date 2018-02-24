#pragma once
#include "stream.h"

namespace common
{
	class IReadable
	{
	public:
		virtual bool Read(in_stream& stream) = 0;
	};

	class IWriteable
	{
	public:
		virtual bool Write(out_stream& stream) = 0;
	};
}
