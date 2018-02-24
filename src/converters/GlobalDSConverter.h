#pragma once
#include "formats\rage\ragecore.h"
#include "formats\rw\rwcore.h"

#include <stdio.h>

class CGlobalDSConverter
{
public:
	template<class TSrc, class TDest>
	static TDest From(TSrc source_data);
};

template<>
rw::core::clump CGlobalDSConverter::From(rw::core::clump chunk)
{
	rw::core::clump ret_chunk;
	return ret_chunk;
}