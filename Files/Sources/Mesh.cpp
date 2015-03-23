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

using namespace SweepingBirds;



Mesh::Mesh()
	:m_eCurrentAttribute(NONE),
	m_iTrianglesCount(0),
	m_iDimension(3),
	m_pAiMaterial(nullptr),
	m_sName("")
{
	glGenVertexArrays(1, &m_iVAO);
}


Mesh::~Mesh()
{
	glDeleteBuffers(4, m_iVBO);
	glDeleteBuffers(1, &m_iVAO);
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

bool Mesh::fill_vertices_infos(std::string meshName, const aiMesh* assimpMesh)
{
	m_sName = meshName;
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
			m_vUVs.push_back(1 - assimpMesh->mTextureCoords[0][j].y);
		}
	}

	return true;
}

void Mesh::bind()
{
	//Bind the VAO
	glBindVertexArray(m_iVAO);

	
	glGenBuffers(4, m_iVBO);
	// Bind indices and upload data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iVBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vTrianglesList.size() * sizeof(int), m_vTrianglesList.data(), GL_STATIC_DRAW);

	// Bind vertices and upload data
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO[1]);
	glEnableVertexAttribArray(POSITION);
	glVertexAttribPointer(0, m_iDimension, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * m_iDimension, (void*)0);
	glBufferData(GL_ARRAY_BUFFER, m_vVertices.size()* sizeof(float), m_vVertices.data(), GL_STATIC_DRAW);

	if (m_iDimension == 3)
	{
		// Bind normals and upload data
		glBindBuffer(GL_ARRAY_BUFFER, m_iVBO[2]);
		glEnableVertexAttribArray(NORMAL);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, m_vNormals.size()* sizeof(float), m_vNormals.data(), GL_DYNAMIC_DRAW);

		// Bind uv coords and upload data
		glBindBuffer(GL_ARRAY_BUFFER, m_iVBO[3]);
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


void Mesh::set_normals(std::vector<float >& norms)
{
	for (int i = 0; i < norms.size(); i++)
	{
		if (i < m_vNormals.size())
		{
			m_vNormals[i] = norms[i];
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO[2]);
	glEnableVertexAttribArray(NORMAL);
	glBufferSubData(GL_ARRAY_BUFFER, NULL, m_vNormals.size()* sizeof(float), m_vNormals.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
