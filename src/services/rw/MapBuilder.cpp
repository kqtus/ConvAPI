#include "MapBuilder.h"
#include "../../converters/FbxToDff.h"
#include "../../formats/fbx/Common.h"

#define CAPI_DEBUG

CMapBuilder::CMapBuilder()
	: m_Logger(nullptr)
{

}

CMapBuilder::~CMapBuilder()
{
	if (m_Logger)
	{
		delete m_Logger;
	}
}

void CMapBuilder::SetInputDir(const std::string& input_dir)
{
	m_InputDirectory = input_dir;
}

void CMapBuilder::SetOutputDir(const std::string& output_dir)
{
	m_OutputDirectory = output_dir;
}

int CMapBuilder::Build()
{
	InitLogger();
	ScanInputDir();

	Log(ELogKind::INFO, "Found " + std::to_string(m_MeshFilePaths.size()) + " mesh(es).");
	Log(ELogKind::INFO, "Found " + std::to_string(m_TexFilePaths.size()) + " texture(s).");

	BuildModels();
	BuildTextures();
	BuildCollisions();
	BuildArchive();
	BuildDefinitionFiles();

	return true;
}

void CMapBuilder::InitLogger()
{
	auto logger_path = m_OutputDirectory;
	logger_path.concat(LOG_FNAME);

	m_Logger = new CLogger(logger_path.string());
	m_Logger->Init();
}

void CMapBuilder::Log(ELogKind log_kind, const std::string& line)
{
#ifdef CAPI_DEBUG
	m_Logger->WriteLine(log_kind, line);
#endif
}

void CMapBuilder::ScanInputDir()
{
	for (auto& entry : fs::recursive_directory_iterator(m_InputDirectory))
	{
		auto entry_path = entry.path();

		if (!entry_path.has_filename())
		{
			continue;
		}

		auto add_resource_path = [](const fs::v1::path& path, const std::string& rhs_ext, TPathVec& paths_vec)
		{
			if (path.extension() == rhs_ext)
			{
				paths_vec.push_back(path);
			}
		};

		add_resource_path(entry_path, INPUT_MESH_EXT, m_MeshFilePaths);
		add_resource_path(entry_path, INPUT_TEX_EXT, m_TexFilePaths);
	}
}

int CMapBuilder::BuildModels()
{
	Log(ELogKind::INFO, "Building models.");
	for (auto& mesh_path : m_MeshFilePaths)
	{
		Log(ELogKind::INFO, "Processing " + mesh_path.string());
		FbxManager* manager = 0;
		FbxScene* scene = 0;
		InitializeSdkObjects(manager, scene);

		if (LoadScene(manager, scene, mesh_path.string().c_str()))
		{
			auto root_node = scene->GetRootNode();
			auto clump = CConverter::From<FbxNode*, rw::core::clump*>(root_node);
			
			out_stream<EStreamType::BINARY> out_dff_stream;

			auto out_path = m_OutputDirectory;
			out_path.concat(mesh_path.filename().string());
			out_path.replace_extension(OUT_MODEL_EXT);
			
			out_dff_stream.Open(out_path.string().c_str());
			clump->Write(out_dff_stream);
			out_dff_stream.Close();

			Log(ELogKind::INFO, "Clump written succesfully.");
		}
		else
		{
			Log(ELogKind::ERROR, "Could not open " + mesh_path.string());
		}
	}
	return true;
}

int CMapBuilder::BuildTextures()
{
	Log(ELogKind::INFO, "Building textures.");
	for (auto& tex_path : m_TexFilePaths)
	{
		Log(ELogKind::INFO, "Processing " + tex_path.string());
	}
	return true;
}

int CMapBuilder::BuildCollisions()
{
	return true;
}

int CMapBuilder::BuildArchive()
{
	return true;
}

int CMapBuilder::BuildDefinitionFiles()
{
	return true;
}