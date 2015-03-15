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
struct aiMaterial;
namespace SweepingBirds
{

	class TextureManager
	{
	public:
		TextureManager();
		~TextureManager();

		void generate_textures(std::vector<std::string> path);
		void generate_textures(std::string name, std::string diffuse_path, std::string specular_path);
		void create_deferred_texture(std::string name, int width, int height);
		void create_shadow_texture(std::string name, int size);

		const std::map< aiTextureType, GLuint >& get_textures(std::string name);
		GLuint get_texture(std::string name, aiTextureType type);

		void Color4f(const aiColor4D *color);
		void set_float4(float f[4], float a, float b, float c, float d);
		void color4_to_float4(const aiColor4D *c, float f[4]);
		void apply_material(const aiMaterial *mtl /*, std::map<std::string, GLuint*> &textureIdMap*/);

	private:
		std::map<std::string, std::map<aiTextureType, GLuint > > m_mTextures;
	};

}