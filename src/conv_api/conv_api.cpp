// conv_api.cpp : Defines the entry point for the console application.
//

#include "conv_api\conv_api_pch.h"
#include <cstdio>
#include <cstdlib>

#include "formats\rw\rwcore.h"
#include "converters\GlobalDSConverter.h"

void WriteTestDff(const char* fname)
{
	out_stream ostr;
	ostr.Open(fname);
	uint32_t section_type = 10;
	uint32_t section_size = 10;
	uint32_t version = 10;
	uint32_t object_count = 10;
	uint32_t pad = 0;

	ostr.Write(&section_type);
	ostr.Write(&section_size);
	ostr.Write(&version);
	ostr.Write(&object_count);
	ostr.Write(&pad);
	ostr.Close();
}

void ReadTestDff(const char* fname)
{
	in_stream dff_stream;
	dff_stream.Open(fname);

	rw::core::clump clump;
	clump.Read(dff_stream);
	printf(" %s\n", clump.ToString().c_str());
}

void ReadVCDff(const char* fname)
{
	in_stream dff_stream;

	if (dff_stream.Open(fname))
	{
		rw::core::clump dff;
		dff.Read(dff_stream);
		dff_stream.Close();
		printf(" %s\n", dff.ToString().c_str());
	}
}

void TestConvert()
{
	rw::core::clump chunk_a;
	rw::core::clump chunk_b;

	chunk_b = CGlobalDSConverter::From<rw::core::clump, rw::core::clump>(chunk_a);
}

int main()
{
	//WriteTestDff("test.dff");
	//ReadTestDff("test.dff");
	TestConvert();
	ReadVCDff("assets\\dt_bowlsign.dff");
	system("PAUSE");
    return 0;
}

