#include <iostream>
#include <fstream>

#include <stb/stb_image.h>
#include <glew/glew.h>

#include <assimp/Importer.hpp>	//OO version Header!
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>


#include <Mesh.h>
#include <Textured3DObject.h>
#include <UtilityToolKit.h>

Textured3DObject::Textured3DObject()
	:m_sName(""),
	m_fSize(1.0),
	m_fSpacing(1.5),
	m_fRange(0.0),
	m_fSpeed(0.0),
	m_bRotating(false),
	m_bWasRotating(false),
	m_dRotatingStartTime(0.0)
{
	m_pImporter = new Assimp::Importer();
}


Textured3DObject::~Textured3DObject()
{
}

bool Textured3DObject::load_object(std::string path, bool own_format)
{
	bool success = false;
	if (own_format)
	{
		auto mesh = new Mesh();
		success = mesh->load_mesh_from_file(path, m_sName);
		m_vMeshes.push_back(mesh);
	}
	else
	{
		success = load_object(path);
	}
	return success;
}

bool Textured3DObject::load_object(std::string path)
{
	//check if file exists
	std::ifstream fin(path.c_str());
	if (!fin.fail())
	{
		fin.close();
	}
	else
	{
		MessageBox(NULL, ("Couldn't open file: " + path).c_str(), "ERROR", MB_OK | MB_ICONEXCLAMATION);
		UtilityToolKit::logInfo(m_pImporter->GetErrorString());
		return false;
	}

	m_pScene = m_pImporter->ReadFile(path, aiProcessPreset_TargetRealtime_Quality);

	// If the import failed, report it
	if (!m_pScene)
	{
		UtilityToolKit::logInfo(m_pImporter->GetErrorString());
		return false;
	}

	// Now we can access the file's contents.
	UtilityToolKit::logInfo("Import of scene " + path + " succeeded.");

	generate_meshes(m_pScene);

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}

bool  Textured3DObject::generate_meshes(const aiScene *sc)
{
	bool success = false;
	if (sc != nullptr)
	{
		struct MyMaterial aMat;
		GLuint buffer;
		// For each mesh
		for (unsigned int n = 0; n < sc->mNumMeshes; ++n)
		{
			if (sc->mMeshes[n] != nullptr)
			{
				auto mesh = new Mesh();
				success &= mesh->fill_vertices_infos(sc->mMeshes[n]);

				// create material uniform buffer
				aiMaterial *mtl = sc->mMaterials[sc->mMeshes[n]->mMaterialIndex];
				success &= mesh->gen_textures(mtl);
				m_vMeshes.push_back(mesh);
			}
		}
	}
	return success;
}

void Textured3DObject::bind_meshes()
{
	for each (auto mesh in m_vMeshes)
	{
		mesh->bind();
	}

}

bool Textured3DObject::check_start_rotation()
{
	if (m_bWasRotating != m_bRotating)
	{
		m_bWasRotating = m_bRotating;
		return m_bRotating;
	}
	return false;
}

void Textured3DObject::set_textures(const std::map< aiTextureType, GLuint >& textures, int mesh_index)
{
	if (mesh_index < m_vMeshes.size())
	{
		m_vMeshes[mesh_index]->set_textures(textures);
	}
}
