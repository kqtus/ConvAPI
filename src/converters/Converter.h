#pragma once
#include <stdio.h>

class CConverter
{
public:
	template<class TSrc, class TDest>
	static TDest From(TSrc source_data);

	template<class TSrc, class TDest>
	static TDest From(TSrc source_data, int param1);
};
