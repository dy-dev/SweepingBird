#include "stb/stb_image.h"
#include "glew/glew.h"

#include <UtilityToolKit.h>
#include <TextureManager.h>


TextureManager::TextureManager()
{
	m_mTextures.clear();
}


TextureManager::~TextureManager()
{
	for each (auto tex in m_mTextures)
	{
		tex.second.clear();
	}
}
void TextureManager::generate_textures(std::vector<std::string> paths)
{
	int numTexture = paths.size();
	// Create Texture
	auto textures = new GLuint[numTexture];
	glGenTextures(numTexture, textures);
	int x;
	int y;
	int comp;
	int index = 0;
	for each (auto path in paths)
	{
		unsigned char * image = stbi_load(path.c_str(), &x, &y, &comp, 3);

		std::map<aiTextureType, GLuint> texs;

		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, textures[index]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		texs[aiTextureType_DIFFUSE] = textures[index];
		m_mTextures[UtilityToolKit::getFileName(path)] = texs;
		index++;
	}
}

void TextureManager::generate_textures(std::string name, std::string diffuse_path, std::string specular_path)
{
	// Create Texture
	GLuint textures[2];
	glGenTextures(2, textures);
	int x;
	int y;
	int comp;
	unsigned char * diffuse = stbi_load(diffuse_path.c_str(), &x, &y, &comp, 3);
	unsigned char * specular = stbi_load(specular_path.c_str(), &x, &y, &comp, 3);
	std::map<aiTextureType, GLuint> texs;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, diffuse);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texs[aiTextureType_DIFFUSE] = textures[0];

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, specular);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texs[aiTextureType_SPECULAR] = textures[1];

	m_mTextures[name] = texs;

}

void TextureManager::create_deferred_texture(std::string name, int width, int height)
{
	// Texture handles
	GLuint gbufferTextures[3];
	glGenTextures(3, gbufferTextures);
	std::map<aiTextureType, GLuint> texs;
	// Create color texture
	glBindTexture(GL_TEXTURE_2D, gbufferTextures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texs[aiTextureType_DIFFUSE] = gbufferTextures[0];

	// Create normal texture
	glBindTexture(GL_TEXTURE_2D, gbufferTextures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texs[aiTextureType_NORMALS] = gbufferTextures[1];

	// Create depth texture
	glBindTexture(GL_TEXTURE_2D, gbufferTextures[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texs[aiTextureType_DISPLACEMENT] = gbufferTextures[2];

	m_mTextures[name] = texs;
}

void TextureManager::create_shadow_texture(std::string name, int size)
{
	// Texture handles
	GLuint shadowTexture;
	glGenTextures(1, &shadowTexture);
	// Create shadow texture
	glBindTexture(GL_TEXTURE_2D, shadowTexture);
	// Create empty texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	// Bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Color needs to be 0 outside of texture coordinates
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	std::map<aiTextureType, GLuint> tex;
	tex[aiTextureType_AMBIENT] = shadowTexture;
	m_mTextures[name] = tex;
}

const std::map< aiTextureType, GLuint >& TextureManager::get_textures(std::string name)
{
	auto tex = m_mTextures.find(name);

	if (tex != m_mTextures.end())
	{
		return tex->second;
	}
	return *(new std::map< aiTextureType, GLuint >());
}

GLuint TextureManager::get_texture(std::string name, aiTextureType type)
{
	auto textures = m_mTextures.find(name);
	if (textures != m_mTextures.end())
	{
		auto tex = textures->second.find(type);
		if (tex != textures->second.end())
		{
			return tex->second;
		}
	}
	return 0;
}

// Can't send color down as a pointer to aiColor4D because AI colors are ABGR.
void  TextureManager::Color4f(const aiColor4D *color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}

void  TextureManager::set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void  TextureManager::color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void  TextureManager::apply_material(const aiMaterial *mtl/*, std::map<std::string, GLuint*> &textureIdMap*/)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;	// changed: to unsigned

	int texIndex = 0;
	aiString texPath;	//contains filename of texture

	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
	{
		//bind texture	
		unsigned int texId = get_texture(UtilityToolKit::getFileName(texPath.data), aiTextureType_DIFFUSE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);
	}
}
