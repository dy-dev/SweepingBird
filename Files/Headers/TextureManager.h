#pragma once
#include <string>
#include <map>
#include <vector>

#include <assimp/material.h>

#include <glew/glew.h>

/*
enum TextureType
{
	DIFFUSE=0,
	SPECULAR,
	SHADOW,
	COLOR = 0,
	NORMAL,
	DEPTH
};
*/
class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	void generate_textures(std::string name, std::string diffuse_path, std::string specular_path);
	void create_deferred_texture(std::string name, int width, int height);
	void create_shadow_texture(std::string name, int size);
	
	const std::map< aiTextureType, GLuint >& get_textures(std::string name);
	GLuint get_texture(std::string name, aiTextureType type);

private :
	std::map<std::string, std::map<aiTextureType, GLuint > > m_mTextures;
};

