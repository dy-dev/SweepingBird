#include <ole2.h>
#include <xmllite.h>
#include <stdio.h>
#include <shlwapi.h>
#include <sstream>
#include <wchar.h>
#include <string>

#include "stb/stb_image.h"
#include "glew/glew.h"

#pragma warning(disable : 4127)  // conditional expression is constant
#define CHKHR(stmt)             do { hr = (stmt); if (FAILED(hr)) goto CleanUp; } while(0)
#define HR(stmt)                do { hr = (stmt); goto CleanUp; } while(0)
#define SAFE_RELEASE(I)         do { if (I){ I->Release(); } I = NULL; } while(0)


#include <Textured3DObject.h>


Textured3DObject::Textured3DObject()
	:m_sName(""),
	m_eCurrentAttribute(NONE),
	m_iTrianglesCount(0),
	m_fSize(1.0),
	m_fSpacing(1.5),
	m_fRange(0.0),
	m_fSpeed(0.0),
	m_bRotating(false),
	m_bWasRotating(false),
	m_dRotatingStartTime(0.0)
{
}


Textured3DObject::~Textured3DObject()
{
}


HRESULT Textured3DObject::get_attributes(IXmlReader* pReader)
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
					m_sName = std::string(tmpName.begin(), tmpName.end());
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



void Textured3DObject::load_object_from_file(std::string path)
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
			if (FAILED(hr = get_attributes(pReader)))
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
				if (FAILED(hr = get_attributes(pReader)))
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

			if (FAILED(hr = get_attributes(pReader)))
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
}


GLuint Textured3DObject::get_texture(int index)
{
	if (index < m_vTexture.size())
	{
		return m_vTexture.at(index);
	}
	return -1;
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