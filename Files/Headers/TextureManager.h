#pragma once
#include <string>
#include <map>
#include <vector>
#include "glew/glew.h"

enum TextureType
{
	DIFFUSE=0,
	SPECULAR,
	SHADOW,
	COLOR = 0,
	NORMAL,
	DEPTH
};

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	void generate_textures(std::string name, std::string diffuse_path, std::string specular_path);
	void create_deferred_texture(std::string name, int width, int height);
	void create_shadow_texture(std::string name, int size);
	
	const std::vector< GLuint >& get_textures(std::string name);
	GLuint get_texture(std::string name, TextureType index);

private :
	std::map<std::string, std::vector< GLuint > > m_mTextures;
};

