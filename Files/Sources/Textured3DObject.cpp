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
#include <TextureManager.h>
#include <UtilityToolKit.h>

using namespace SweepingBirds;


Textured3DObject::Textured3DObject()
	:m_sName(""),
	m_sPath(""),
	m_fSize(1.0),
	m_fSpacing(1.5),
	m_fRange(0.0),
	m_fSpeed(0.0),
	m_bRotating(false),
	m_bWasRotating(false),
	m_dRotatingStartTime(0.0),
	m_fRotationAngle(0.0f)
{
	m_pImporter = new Assimp::Importer();
}


Textured3DObject::~Textured3DObject()
{
}

bool Textured3DObject::load_object(std::string path, bool own_format, TextureManager * texmgr)
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
		success = load_object(path, texmgr);
	}
	return success;
}

bool Textured3DObject::load_object(std::string path, TextureManager * texmgr)
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
	m_sPath = path;
	m_sName = UtilityToolKit::getFileName(path);
	// If the import failed, report it
	if (!m_pScene)
	{
		UtilityToolKit::logInfo(m_pImporter->GetErrorString());
		return false;
	}

	// Now we can access the file's contents.
	UtilityToolKit::logInfo("Import of scene " + path + " succeeded.");

	generate_meshes();
	generate_textures(texmgr);
	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}

bool Textured3DObject::generate_meshes()
{
	bool success = false;
	if (m_pScene != nullptr)
	{
		struct MyMaterial aMat;
		GLuint buffer;
		// For each mesh
		for (unsigned int n = 0; n < m_pScene->mNumMeshes; ++n)
		{
			if (m_pScene->mMeshes[n] != nullptr)
			{
				auto mesh = new Mesh();
				std::string childName = "";
				if (n < m_pScene->mRootNode->mNumChildren)
				{
					childName = m_pScene->mRootNode->mChildren[n]->mName.data;
				}
				success &= mesh->fill_vertices_infos(childName, m_pScene->mMeshes[n]);

				// create material uniform buffer
				aiMaterial *mtl = m_pScene->mMaterials[m_pScene->mMeshes[n]->mMaterialIndex];
				mesh->set_material(mtl);
				m_vMeshes.push_back(mesh);
			}
		}
	}
	return success;
}

bool Textured3DObject::generate_textures(TextureManager * texmgr)
{
	/* scan scene's materials for textures */
	for (unsigned int m = 0; m < m_pScene->mNumMaterials; ++m)
	{
		int texIndex = 0;
		aiString path;	// filename
		aiReturn texFound = m_pScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
		while (texFound == AI_SUCCESS)
		{
			//fill map with textures, OpenGL image ids set to 0
			textureIdMap[path.data] = 0;
			char myPath[_MAX_PATH + 1];
			GetModuleFileName(NULL, myPath, _MAX_PATH);
			auto textureBaseName = UtilityToolKit::getBasePath(std::string(myPath)) + UtilityToolKit::getBasePath(m_sPath);
			m_vTexturePath.push_back(std::make_pair(textureBaseName + std::string(path.data), aiTextureType_DIFFUSE));
			if (m_pScene->mMaterials[m]->GetTexture(aiTextureType_SPECULAR, texIndex, &path) == AI_SUCCESS)
			{
				m_vTexturePath.push_back(std::make_pair(textureBaseName + std::string(path.data), aiTextureType_SPECULAR));
			}
			if (m_pScene->mMaterials[m]->GetTexture(aiTextureType_AMBIENT, texIndex, &path) == AI_SUCCESS)
			{
				m_vTexturePath.push_back(std::make_pair(textureBaseName + std::string(path.data), aiTextureType_AMBIENT));
			}
			if (m_pScene->mMaterials[m]->GetTexture(aiTextureType_OPACITY, texIndex, &path) == AI_SUCCESS)
			{
				m_vTexturePath.push_back(std::make_pair(textureBaseName + std::string(path.data), aiTextureType_OPACITY));
			}
			if (m_pScene->mMaterials[m]->GetTexture(aiTextureType_DISPLACEMENT, texIndex, &path) == AI_SUCCESS)
			{
				m_vTexturePath.push_back(std::make_pair(textureBaseName + std::string(path.data), aiTextureType_DISPLACEMENT));
			}
			if (m_pScene->mMaterials[m]->GetTexture(aiTextureType_SHININESS, texIndex, &path) == AI_SUCCESS)
			{
				m_vTexturePath.push_back(std::make_pair(textureBaseName + std::string(path.data), aiTextureType_SHININESS));
			}

			// more textures?
			texIndex++;
			texFound = m_pScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
		}
	}

	texmgr->generate_textures(m_vTexturePath);

	return true;
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
