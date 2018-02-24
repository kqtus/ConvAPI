// conv_api.cpp : Defines the entry point for the console application.
//

#include "conv_api\conv_api_pch.h"
#include <cstdio>
#include <cstdlib>

#include "formats\rw\rwcore.h"
#include "formats\xml\node.h"

#include "converters\DffToXml.h"

void WriteTestDff(const char* fname)
{
	out_stream<EStreamType::BINARY> ostr;
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
	in_stream<EStreamType::BINARY> dff_stream;
	dff_stream.Open(fname);

	rw::core::clump clump;
	clump.Read(dff_stream);
	printf(" %s\n", clump.ToString().c_str());
}

void ReadVCDff(const char* fname)
{
	in_stream<EStreamType::BINARY> dff_stream;

	if (dff_stream.Open(fname))
	{
		rw::core::clump dff;
		dff.Read(dff_stream);
		dff_stream.Close();
		printf(" %s\n", dff.ToString().c_str());
	}
}

void TestDffToXml(const char* file_name, const char* out_file_name)
{
	printf("Converting %s to %s...\n", file_name, out_file_name);
	in_stream<EStreamType::BINARY> dff_stream;

	if (dff_stream.Open(file_name))
	{
		rw::core::clump chunk_a;
		chunk_a.Read(dff_stream);
		dff_stream.Close();

		xml::node node = CConverter::From<rw::core::clump, xml::node>(chunk_a);

		out_stream<EStreamType::TEXT> xml_stream;
		xml_stream.Open(out_file_name);
		node.Write(xml_stream);
		xml_stream.Close();

		printf("Conversion ended successfully.\n");
	}	
	else
	{
		printf("Failed to convert.\n");
	}
}

int main()
{
	//WriteTestDff("test.dff");
	//ReadTestDff("test.dff");
	TestDffToXml("assets\\dt_bowlsign.dff", "assets\\dt_bowlsign.xml");
	//ReadVCDff("assets\\dt_bowlsign.dff");
	system("PAUSE");
    return 0;
}

