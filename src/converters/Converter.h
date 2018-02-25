#pragma once
#include <stdio.h>

class CConverter
{
public:
	template<class TSrc, class TDest>
	static TDest From(const TSrc& source_data);
};
