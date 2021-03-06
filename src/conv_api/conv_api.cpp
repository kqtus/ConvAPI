// conv_api.cpp : Defines the entry point for the console application.
//

#include "conv_api\conv_api_pch.h"
#include <cstdio>
#include <cstdlib>
#include <filesystem>

#include "formats\rw\rwcore.h"
#include "formats\xml\node.h"
#include "formats\rw\rwrs.h"

#include "converters\DffToXml.h"
#include "services\Logger.h"

#include "services\rw\MapBuilder.h"

//#include "converters\FbxToDff.h"

namespace fs = std::experimental::filesystem;

void ConvertDffToXml(const char* in_path, const char* out_path)
{
	fs::path dff_path(in_path);
	fs::path xml_path(out_path);

	printf("Converting %s to %s... ", 
		dff_path.filename().string().c_str(), 
		xml_path.filename().string().c_str()
	);

	in_stream<EStreamType::BINARY> dff_stream;

	if (dff_stream.Open(in_path))
	{
		auto chunk_a = new rw::core::clump();
		chunk_a->Read(dff_stream);
		dff_stream.Close();

		auto node = CConverter::From<rw::core::clump*, xml::node*>(chunk_a);

		out_stream<EStreamType::TEXT> xml_stream;
		xml_stream.Open(out_path);
		node->Write(xml_stream);
		xml_stream.Close();
		
		printf("OK\n");
	}	
	else
	{
		printf("FAIL\n");
	}
}

void TestWriteEmptyDff(const char* out_path)
{
	out_stream<EStreamType::BINARY> dff_stream;

	printf("Saving empty model to %s... ", out_path);
	if (dff_stream.Open(out_path))
	{
		auto chunk = new rw::core::clump();
		chunk->Write(dff_stream);
		dff_stream.Close();

		printf("OK\n");
	}
	else 
	{
		printf("FAIL\n");
	}
}

void TestReadAndSaveDff(const char* in_path, const char* out_path)
{
	CLogger logger("resave_dff.log");
	logger.Init();

	fs::path dff_path(in_path);
	fs::path xml_path(out_path);

	printf("Converting %s to %s... ",
		dff_path.filename().string().c_str(),
		xml_path.filename().string().c_str()
	);

	logger.WriteLine(ELogKind::INFO, "Converting files...");

	in_stream<EStreamType::BINARY> in_dff_stream;

	if (in_dff_stream.Open(in_path))
	{
		auto chunk_a = new rw::core::clump();
		chunk_a->Read(in_dff_stream);
		in_dff_stream.Close();

		out_stream<EStreamType::BINARY> out_dff_stream;
		out_dff_stream.Open(out_path);
		chunk_a->Write(out_dff_stream);
		out_dff_stream.Close();

		printf("OK\n");
	}
	else
	{
		printf("FAIL\n");
	}
}

bool SaveNode(xml::node* node, const char* file_path)
{
	std::string out_path(file_path);
	out_stream<EStreamType::TEXT> xml_stream;

	if (!xml_stream.Open(out_path.c_str()))
	{
		return false;
	}

	node->Write(xml_stream);
	xml_stream.Close();
	return true;
}

void TestOpenImg()
{
	using TInBinStream = in_stream<EStreamType::BINARY>;
	using TOutTextStream = out_stream<EStreamType::TEXT>;

	TInBinStream inp;

	inp.Open("C:\\Program Files (x86)\\Steam\\steamapps\\common\\Grand Theft Auto Vice City\\models\\gta3.img");

	auto img = new rw::rs::archive<rw::rs::EArchiveVer::VER1>();
	img->Read(inp);

	for (auto& entry_hd : img->GetEntryHeaders())
	{
		std::string file_name((char*)entry_hd->file_name);
		printf("%s\n", file_name.c_str());

		if (file_name.find(".dff") == std::string::npos)
		{
			continue;
		}

		auto dff = new rw::core::clump();
		TInBinStream dff_stream;

		if (!img->GetFileStream(dff_stream, file_name))
		{
			break;
		}

		dff->Read(dff_stream);

		auto node = CConverter::From<rw::core::clump*, xml::node*>(dff);
		SaveNode(node, std::string("C:\\Users\\Marek\\Desktop\\extracted_data\\" + file_name + ".xml").c_str());
	}
}

void BuildMap(const std::string& input_path, const std::string& output_path)
{
	CMapBuilder builder;
	builder.SetInputDir(input_path);
	builder.SetOutputDir(output_path);
	builder.Build();
}

int _main()
{
	// Vice City DFF test
	//ConvertDffToXml("assets\\dff\\vc\\dt_bowlsign.dff", "assets\\xml\\vc\\dt_bowlsign.xml");
	//ConvertDffToXml("assets\\dff\\vc\\sabre.dff", "assets\\xml\\vc\\sabre.xml");

	// San Andreas DFF test
	//ConvertDffToXml("assets\\dff\\sa\\sv_ground_04_sfs.dff", "assets\\xml\\sa\\sv_ground_04_sfs.xml");
	
	// Test saving empty dff
	//TestWriteEmptyDff("assets\\dff\\sa\\empty.dff");

	// Test resave dff
	TestReadAndSaveDff("assets\\dff\\vc\\sabre.dff", "assets\\dff\\vc\\sabre_resaved.dff");
	/*TestReadAndSaveDff
	(
		"assets\\dff\\sa\\sabre.dff", 
		"assets\\dff\\sa\\sabre_resaved.dff"
	);*/
	//TestOpenImg();

	BuildMap("assets\\fbx\\", "assets\\build_res\\");
	system("PAUSE");
    return 0;
}

#include <windows.h>
#include "App/D3DApp.h"

#include "renderer/Renderer/Renderer.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd
)
{
	CD3DApp app(hInstance);

	if (app.Init())
	{
		ShowWindow(app.GetMainWindowHandler(), nShowCmd);
		UpdateWindow(app.GetMainWindowHandler());
		app.Run();
	}
}