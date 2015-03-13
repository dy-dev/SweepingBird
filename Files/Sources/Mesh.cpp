#include <ole2.h>
#include <xmllite.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <shlwapi.h>
#include <sstream>
#include <wchar.h>
#include <string>

#include <stb/stb_image.h>
#include <glew/glew.h>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/LogStream.hpp>

#include <Mesh.h>

#pragma warning(disable : 4127)  // conditional expression is constant
#define CHKHR(stmt)             do { hr = (stmt); if (FAILED(hr)) goto CleanUp; } while(0)
#define HR(stmt)                do { hr = (stmt); goto CleanUp; } while(0)
#define SAFE_RELEASE(I)         do { if (I){ I->Release(); } I = NULL; } while(0)


Mesh::Mesh()
	:m_eCurrentAttribute(NONE),
	m_iTrianglesCount(0),
	m_iDimension(3)
{
	glGenVertexArrays(1, &m_iVAO);
}


Mesh::~Mesh()
{
}

HRESULT Mesh::get_attributes(IXmlReader* pReader, std::string &name)
{
	const WCHAR* pwszPrefix;
	const WCHAR* pwszLocalName;
	const WCHAR* pwszValue;
	HRESULT hr = pReader->MoveToFirstAttribute();

	if (S_FALSE == hr)
		return hr;
	if (S_OK != hr)
	{
		wprintf(L"Error moving to first attribute, error is %08.8lx", hr);
		return hr;
	}
	else
	{
		while (TRUE)
		{
			if (!pReader->IsDefault())
			{
				UINT cwchPrefix;
				if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
				{
					wprintf(L"Error getting prefix, error is %08.8lx", hr);
					return hr;
				}
				if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
				{
					wprintf(L"Error getting local name, error is %08.8lx", hr);
					return hr;
				}
				if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
				{
					wprintf(L"Error getting value, error is %08.8lx", hr);
					return hr;
				}
				if (StrCmpW(pwszLocalName, L"name") == 0)
				{
					std::wstring tmpName;
					std::wstringstream valueToConvert;

					valueToConvert << pwszValue;
					valueToConvert >> tmpName;
					name = std::string(tmpName.begin(), tmpName.end());
				}
				if (m_eCurrentAttribute == TRIANGLES && StrCmpW(pwszLocalName, L"count") == 0)
				{
					std::wstringstream valueToConvert;

					valueToConvert << pwszValue;
					valueToConvert >> m_iTrianglesCount;
				}
				else if (m_eCurrentAttribute == VERTICES && StrCmpW(pwszLocalName, L"dimension") == 0)
				{
					std::wstringstream valueToConvert;

					valueToConvert << pwszValue;
					valueToConvert >> m_iDimension;
				}
				if (cwchPrefix > 0)
					wprintf(L"Attr: %s:%s=\"%s\" \n", pwszPrefix, pwszLocalName, pwszValue);
				else
					wprintf(L"Attr: %s=\"%s\" \n", pwszLocalName, pwszValue);
			}

			if (S_OK != pReader->MoveToNextAttribute())
				break;
		}
	}
	return hr;
}

bool Mesh::load_mesh_from_file(std::string path, std::string &name)
{
	HRESULT hr = S_OK;
	IStream *pFileStream = NULL;
	IXmlReader *pReader = NULL;
	XmlNodeType nodeType;
	const WCHAR* pwszPrefix;
	const WCHAR* pwszLocalName;
	const WCHAR* pwszValue;
	UINT cwchPrefix;

	//read until there are no more nodes
	wchar_t* currentValue = nullptr;
	std::wstringstream valueToConvert;
	std::wstring tmpString;
	int tmpInt = 0;
	float tmpFloat = 0.0f;

	//Open read-only input stream
	if (FAILED(hr = SHCreateStreamOnFile(path.c_str(), STGM_READ, &pFileStream)))
	{
		wprintf(L"Error creating file reader, error is %08.8lx", hr);
		HR(hr);
	}

	if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**)&pReader, NULL)))
	{
		wprintf(L"Error creating xml reader, error is %08.8lx", hr);
		HR(hr);
	}

	if (FAILED(hr = pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
	{
		wprintf(L"Error setting XmlReaderProperty_DtdProcessing, error is %08.8lx", hr);
		HR(hr);
	}

	if (FAILED(hr = pReader->SetInput(pFileStream)))
	{
		wprintf(L"Error setting input for reader, error is %08.8lx", hr);
		HR(hr);
	}


	while (S_OK == (hr = pReader->Read(&nodeType)))
	{
		switch (nodeType)
		{
		case XmlNodeType_XmlDeclaration:
			wprintf(L"XmlDeclaration\n");
			if (FAILED(hr = get_attributes(pReader, name)))
			{
				wprintf(L"Error writing attributes, error is %08.8lx", hr);
				HR(hr);
			}
			break;
		case XmlNodeType_Element:
			if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
			{
				wprintf(L"Error getting prefix, error is %08.8lx", hr);
				HR(hr);
			}
			if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
			{
				wprintf(L"Error getting local name, error is %08.8lx", hr);
				HR(hr);
			}
			if (StrCmpW(pwszLocalName, L"Object") == 0)
			{
				if (FAILED(hr = get_attributes(pReader, name)))
				{
					wprintf(L"Error writing attributes, error is %08.8lx", hr);
					HR(hr);
				}
			}
			if (StrCmpW(pwszLocalName, L"triangles") == 0)
			{
				m_eCurrentAttribute = TRIANGLES;
			}
			else if (StrCmpW(pwszLocalName, L"uvs") == 0)
			{
				m_eCurrentAttribute = UVS;
			}
			if (StrCmpW(pwszLocalName, L"vertices") == 0)
			{
				m_eCurrentAttribute = VERTICES;
			}
			if (StrCmpW(pwszLocalName, L"normals") == 0)
			{
				m_eCurrentAttribute = NORMALS;
			}
			if (cwchPrefix > 0)
				wprintf(L"Element: %s:%s\n", pwszPrefix, pwszLocalName);
			else
				wprintf(L"Element: %s\n", pwszLocalName);

			if (FAILED(hr = get_attributes(pReader, name)))
			{
				wprintf(L"Error writing attributes, error is %08.8lx", hr);
				HR(hr);
			}

			if (pReader->IsEmptyElement())
				wprintf(L" (empty)");
			break;
		case XmlNodeType_EndElement:
			if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
			{
				wprintf(L"Error getting prefix, error is %08.8lx", hr);
				HR(hr);
			}
			if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
			{
				wprintf(L"Error getting local name, error is %08.8lx", hr);
				HR(hr);
			}
			if (cwchPrefix > 0)
				wprintf(L"End Element: %s:%s\n", pwszPrefix, pwszLocalName);
			else
				wprintf(L"End Element: %s\n", pwszLocalName);
			break;
		case XmlNodeType_Text:
		case XmlNodeType_Whitespace:
			if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
			{
				wprintf(L"Error getting value, error is %08.8lx", hr);
				HR(hr);
			}
			currentValue = std::wcstok((wchar_t*)(pwszValue), L",");
			while (currentValue != nullptr)
			{
				valueToConvert << currentValue;
				valueToConvert >> tmpString;

				if (tmpString.find(L"f") != std::string::npos)
				{
					tmpString.erase(tmpString.find(L"f"), tmpString.length());

				}
				/*if (tmpString.find(L".") != std::string::npos)
				{
				tmpString = tmpString.replace(tmpString.find(L"."), 1, L",");
				}*/
				valueToConvert.clear();
				valueToConvert << tmpString;

				switch (m_eCurrentAttribute)
				{
				case TRIANGLES:
					valueToConvert >> tmpInt;
					m_vTrianglesList.push_back(tmpInt);
					break;
				case UVS:
					valueToConvert >> tmpFloat;
					m_vUVs.push_back(tmpFloat);
					break;
				case VERTICES:
					valueToConvert >> tmpFloat;
					m_vVertices.push_back(tmpFloat);
					break;
				case NORMALS:
					valueToConvert >> tmpFloat;
					m_vNormals.push_back(tmpFloat);
					break;
				default:
					break;
				}
				currentValue = std::wcstok(NULL, L",");
				valueToConvert.clear();
				tmpString.clear();
			}
			wprintf(L"Text: >%s<\n", pwszValue);
			m_eCurrentAttribute = NONE;
			break;
		case XmlNodeType_CDATA:
			if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
			{
				wprintf(L"Error getting value, error is %08.8lx", hr);
				HR(hr);
			}
			wprintf(L"CDATA: %s\n", pwszValue);
			break;
		case XmlNodeType_ProcessingInstruction:
			if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
			{
				wprintf(L"Error getting name, error is %08.8lx", hr);
				HR(hr);
			}
			if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
			{
				wprintf(L"Error getting value, error is %08.8lx", hr);
				HR(hr);
			}
			wprintf(L"Processing Instruction name:%s value:%s\n", pwszLocalName, pwszValue);
			break;
		case XmlNodeType_Comment:
			if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
			{
				wprintf(L"Error getting value, error is %08.8lx", hr);
				HR(hr);
			}
			wprintf(L"Comment: %s\n", pwszValue);
			break;
		case XmlNodeType_DocumentType:
			wprintf(L"DOCTYPE is not printed\n");
			break;
		}
	}

CleanUp:

	SAFE_RELEASE(pFileStream);
	SAFE_RELEASE(pReader);

	return true;
}

//int LoadGLTextures(const aiScene* scene)
//{
//	ILboolean success;
//
//	/* initialization of DevIL */
//	ilInit();
//
//	/* scan scene's materials for textures */
//	for (unsigned int m = 0; m<scene->mNumMaterials; ++m)
//	{
//		int texIndex = 0;
//		aiString path;	// filename
//
//		aiReturn texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
//		while (texFound == AI_SUCCESS) {
//			//fill map with textures, OpenGL image ids set to 0
//			textureIdMap[path.data] = 0;
//			// more textures?
//			texIndex++;
//			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
//		}
//	}
//
//	int numTextures = textureIdMap.size();
//
//	/* create and fill array with DevIL texture ids */
//	ILuint* imageIds = new ILuint[numTextures];
//	ilGenImages(numTextures, imageIds);
//
//	/* create and fill array with GL texture ids */
//	GLuint* textureIds = new GLuint[numTextures];
//	glGenTextures(numTextures, textureIds); /* Texture name generation */
//
//	/* get iterator */
//	std::map<std::string, GLuint>::iterator itr = textureIdMap.begin();
//	int i = 0;
//	for (; itr != textureIdMap.end(); ++i, ++itr)
//	{
//		//save IL image ID
//		std::string filename = (*itr).first;  // get filename
//		(*itr).second = textureIds[i];	  // save texture id for filename in map
//
//		ilBindImage(imageIds[i]); /* Binding of DevIL image name */
//		ilEnable(IL_ORIGIN_SET);
//		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
//		success = ilLoadImage((ILstring)filename.c_str());
//
//		if (success) {
//			/* Convert image to RGBA */
//			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
//
//			/* Create and load textures to OpenGL */
//			glBindTexture(GL_TEXTURE_2D, textureIds[i]);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
//				ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE,
//				ilGetData());
//		}
//		else
//			printf("Couldn't load Image: %s\n", filename.c_str());
//	}
//	/* Because we have already copied image data into texture data
//	we can release memory used by image. */
//	ilDeleteImages(numTextures, imageIds);
//
//	//Cleanup
//	delete[] imageIds;
//	delete[] textureIds;
//
//	//return success;
//	return true;
//}
bool Mesh::fill_vertices_infos(const aiMesh* assimpMesh)
{
	for (unsigned int t = 0; t < assimpMesh->mNumFaces; ++t)
	{
		const aiFace* face = &assimpMesh->mFaces[t];
		for (int i = 0; i < face->mNumIndices; i++)
		{
			m_vTrianglesList.push_back(face->mIndices[i]);
		}
	}

	m_iTrianglesCount += assimpMesh->mNumFaces;
	if (assimpMesh->HasPositions())
	{
		for (int j = 0; j < assimpMesh->mNumVertices; j++)
		{
			m_vVertices.push_back(assimpMesh->mVertices[j].x);
			m_vVertices.push_back(assimpMesh->mVertices[j].y);
			m_vVertices.push_back(assimpMesh->mVertices[j].z);
		}
	}

	if (assimpMesh->HasNormals())
	{
		for (int j = 0; j < assimpMesh->mNumVertices; j++)
		{
			m_vNormals.push_back(assimpMesh->mNormals[j].x);
			m_vNormals.push_back(assimpMesh->mNormals[j].y);
			m_vNormals.push_back(assimpMesh->mNormals[j].z);
		}
	}

	if (assimpMesh->HasTextureCoords(0))
	{
		for (int j = 0; j < assimpMesh->mNumVertices; j++)
		{
			m_vUVs.push_back(assimpMesh->mTextureCoords[0][j].x);
			m_vUVs.push_back(assimpMesh->mTextureCoords[0][j].y);
		}
	}

	return true;
}

bool Mesh::gen_textures(aiMaterial * material)
{
	struct MyMaterial aMat;
	aiString texPath;	//contains filename of texture
	if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)){
		//bind texture
		//unsigned int texId = textureIdMap[texPath.data];
		//m_vTexture.push_back(texId);
		aMat.texCount = 1;
	}
	else
	{
		aMat.texCount = 0;
	}

	//float c[4];
	//set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	//aiColor4D diffuse;
	//if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
	//	color4_to_float4(&diffuse, c);
	//memcpy(aMat.diffuse, c, sizeof(c));

	//set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	//aiColor4D ambient;
	//if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
	//	color4_to_float4(&ambient, c);
	//memcpy(aMat.ambient, c, sizeof(c));

	//set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	//aiColor4D specular;
	//if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
	//	color4_to_float4(&specular, c);
	//memcpy(aMat.specular, c, sizeof(c));

	//set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	//aiColor4D emission;
	//if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
	//	color4_to_float4(&emission, c);
	//memcpy(aMat.emissive, c, sizeof(c));

	//float shininess = 0.0;
	//unsigned int max;
	//aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	//aMat.shininess = shininess;

	//glGenBuffers(1, &(aMesh.uniformBlockIndex));
	//glBindBuffer(GL_UNIFORM_BUFFER, aMesh.uniformBlockIndex);
	//glBufferData(GL_UNIFORM_BUFFER, sizeof(aMat), (void *)(&aMat), GL_STATIC_DRAW);

	//myMeshes.push_back(aMesh);
	return true;
}

void Mesh::bind()
{
	//Bind the VAO
	glBindVertexArray(m_iVAO);

	GLuint vbo[4];
	glGenBuffers(4, vbo);
	// Bind indices and upload data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vTrianglesList.size() * sizeof(int), m_vTrianglesList.data(), GL_STATIC_DRAW);

	// Bind vertices and upload data
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glEnableVertexAttribArray(POSITION);
	glVertexAttribPointer(0, m_iDimension, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * m_iDimension, (void*)0);
	glBufferData(GL_ARRAY_BUFFER, m_vVertices.size()* sizeof(float), m_vVertices.data(), GL_STATIC_DRAW);

	if (m_iDimension == 3)
	{
		// Bind normals and upload data
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glEnableVertexAttribArray(NORMAL);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, m_vNormals.size()* sizeof(float), m_vNormals.data(), GL_STATIC_DRAW);

		// Bind uv coords and upload data
		glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
		glEnableVertexAttribArray(TEXCOORD);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, m_vUVs.size()* sizeof(float), m_vUVs.data(), GL_STATIC_DRAW);
	}
	// Unbind everything
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint Mesh::get_texture(aiTextureType type)
{
	auto textId = m_mTexture.find(type);
	if (textId != m_mTexture.end())
	{
		return textId->second;
	}
	return 0;
}

// Render Assimp Model
//
//void recursive_render(const aiScene *sc, const aiNode* nd)
//{
//
//	// Get node transformation matrix
//	aiMatrix4x4 m = nd->mTransformation;
//	// OpenGL matrices are column major
//	m.Transpose();
//
//	// save model matrix and apply node transformation
//	pushMatrix();
//
//	float aux[16];
//	memcpy(aux, &m, sizeof(float) * 16);
//	multMatrix(modelMatrix, aux);
//	setModelMatrix();
//
//
//	// draw all meshes assigned to this node
//	for (unsigned int n = 0; n < nd->mNumMeshes; ++n){
//		// bind material uniform
//		glBindBufferRange(GL_UNIFORM_BUFFER, materialUniLoc, myMeshes[nd->mMeshes[n]].uniformBlockIndex, 0, sizeof(struct MyMaterial));
//		// bind texture
//		glBindTexture(GL_TEXTURE_2D, myMeshes[nd->mMeshes[n]].texIndex);
//		// bind VAO
//		glBindVertexArray(myMeshes[nd->mMeshes[n]].vao);
//		// draw
//		glDrawElements(GL_TRIANGLES, myMeshes[nd->mMeshes[n]].numFaces * 3, GL_UNSIGNED_INT, 0);
//
//	}
//
//	// draw all children
//	for (unsigned int n = 0; n < nd->mNumChildren; ++n){
//		recursive_render(sc, nd->mChildren[n]);
//	}
//	popMatrix();
//}
//
//
//void renderScene(void) {
//
//	static float step = 0.0f;
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	// set camera matrix
//	setCamera(camX, camY, camZ, 0, 0, 0);
//
//	// set the model matrix to the identity Matrix
//	setIdentityMatrix(modelMatrix, 4);
//
//	// sets the model matrix to a scale matrix so that the model fits in the window
//	scale(scaleFactor, scaleFactor, scaleFactor);
//
//	// keep rotating the model
//	rotate(step, 0.0f, 1.0f, 0.0f);
//
//	// use our shader
//	glUseProgram(program);
//
//	// we are only going to use texture unit 0
//	// unfortunately samplers can't reside in uniform blocks
//	// so we have set this uniform separately
//	glUniform1i(texUnit, 0);
//
//	recursive_render(scene, scene->mRootNode);
//
//	// FPS computation and display
//	frame++;
//	time = glutGet(GLUT_ELAPSED_TIME);
//	if (time - timebase > 1000) {
//		sprintf(s, "FPS:%4.2f",
//			frame*1000.0 / (time - timebase));
//		timebase = time;
//		frame = 0;
//		glutSetWindowTitle(s);
//	}
//
//	// swap buffers
//	glutSwapBuffers();
//
//	// increase the rotation angle
//	//step++;
//}
//
