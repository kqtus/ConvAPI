#pragma once
#include <string>
#include <vector>
//#include "FbxToDff.h"

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
public:
	CMapBuilder();
	~CMapBuilder();

	void SetInputDir(const std::string& input_dir);
	void SetOutputDir(const std::string& output_dir);

	int Build();

protected:
	int BuildModels();
	int BuildTextures();
	int BuildCollisions();
	int BuildArchive();
	int BuildDefinitionFiles();

	std::string m_InputDirectory;
	std::string m_OutputDirectory;
};