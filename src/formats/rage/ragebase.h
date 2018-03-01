#pragma once
#include "../formats_pch.h"
#include "../../utilities/stream.h"
#include "../../utilities/interfaces.h"

namespace rage
{
	class block_base : public common::IBinReadable
	{
		CONVERTIBLE_ENTITY
	public:
		bool Read(in_stream<EStreamType::BINARY>& stream) override;
	};

	template<class T>
	class ptr : public common::IBinReadable
	{
		CONVERTIBLE_ENTITY
	public:
		bool Read(in_stream<EStreamType::BINARY>& stream) override;
	};
}