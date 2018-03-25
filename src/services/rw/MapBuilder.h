#pragma once
#include <string>
#include <vector>
#include <experimental\filesystem>
#include "../Logger.h"

//#include "FbxToDff.h"

namespace fs = std::experimental::filesystem;

/*
	MapBuilder is responsible for building map parts.
	It takes FBX files along with textures and produces
	following types of data:
		- dff: model
		- txd: texture dictionary
		- col: colision mesh
		- img: data archive
		- ide: item definitions file
		- ipl: item placement file
*/
class CMapBuilder
{
	using TPathVec = std::vector<fs::v1::path>;

	const std::string INPUT_MESH_EXT = ".fbx";
	const std::string INPUT_TEX_EXT = ".dds";
	const std::string OUT_MODEL_EXT = ".dff";
	const std::string LOG_FNAME = "map_builder.log";
public:
	CMapBuilder();
	~CMapBuilder();

	void SetInputDir(const std::string& input_dir);
	void SetOutputDir(const std::string& output_dir);

	int Build();

protected:
	void InitLogger();
	void Log(ELogKind log_kind, const std::string& line);
	void ScanInputDir();

	int BuildModels();
	int BuildTextures();
	int BuildCollisions();
	int BuildArchive();
	int BuildDefinitionFiles();

	fs::v1::path m_InputDirectory;
	fs::v1::path m_OutputDirectory;
	TPathVec m_MeshFilePaths;
	TPathVec m_TexFilePaths;

	CLogger* m_Logger;
};