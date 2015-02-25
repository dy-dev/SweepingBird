#include "stb/stb_image.h"
#include "glew/glew.h"

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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, diffuse);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, specular);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_mTextures[name] = std::vector<GLuint>(textures, textures + 2);

}

void TextureManager::create_deferred_texture(std::string name, int width, int height)
{
	// Texture handles
	GLuint gbufferTextures[3];
	glGenTextures(3, gbufferTextures);

	// Create color texture
	glBindTexture(GL_TEXTURE_2D, gbufferTextures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Create normal texture
	glBindTexture(GL_TEXTURE_2D, gbufferTextures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Create depth texture
	glBindTexture(GL_TEXTURE_2D, gbufferTextures[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	m_mTextures[name] = std::vector<GLuint>(gbufferTextures, gbufferTextures + 3);
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
	m_mTextures[name] = std::vector<GLuint>(shadowTexture);
}

const std::vector< GLuint >& TextureManager::get_textures(std::string name)
{
	auto tex = m_mTextures.find(name);

	if (tex != m_mTextures.end())
	{
		return tex->second;
	}
	return std::vector<GLuint>();
}

GLuint TextureManager::get_texture(std::string name, TextureType index)
{ 
	auto tex = m_mTextures.find(name);
	if (tex != m_mTextures.end() && tex->second.size() > (int)index)
	{
		return tex->second.at(index);
	}
	return -1; 
}
