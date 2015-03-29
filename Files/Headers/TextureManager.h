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
    /**
     * @fn  TextureManager::TextureManager();
     *
     * @brief Default constructor.
     */
		TextureManager();

    /**
     * @fn  TextureManager::~TextureManager();
     *
     * @brief Destructor.
     */
		~TextureManager();

    /**
     * @fn  void TextureManager::generate_textures(std::vector<std::pair<std::string, aiTextureType> > path);
     *
     * @brief Generates the textures.
     *
     * @param path  Full pathname of the file.
     */
		void generate_textures(std::vector<std::pair<std::string, aiTextureType> > path);

    /**
     * @fn  void TextureManager::generate_textures(std::string name, std::string diffuse_path, std::string specular_path);
     *
     * @brief Generates the textures.
     *
     * @param name          The name.
     * @param diffuse_path  Full pathname of the diffuse file.
     * @param specular_path Full pathname of the specular file.
     */
		void generate_textures(std::string name, std::string diffuse_path, std::string specular_path);

    /**
     * @fn  void TextureManager::create_deferred_texture(std::string name, int width, int height);
     *
     * @brief Creates deferred texture.
     *
     * @param name    The name.
     * @param width   The width.
     * @param height  The height.
     */
		void create_deferred_texture(std::string name, int width, int height);

    /**
     * @fn  void TextureManager::create_shadow_texture(std::string name, int size);
     *
     * @brief Creates shadow texture.
     *
     * @param name  The name.
     * @param size  The size.
     */
		void create_shadow_texture(std::string name, int size);

    /**
     * @fn  const std::map< aiTextureType, GLuint >& TextureManager::get_textures(std::string name);
     *
     * @brief Gets the textures.
     *
     * @param name  The name.
     *
     * @return  The textures.
     */
		const std::map< aiTextureType, GLuint >& get_textures(std::string name);

    /**
     * @fn  GLuint TextureManager::get_texture(std::string name, aiTextureType type);
     *
     * @brief Gets a texture.
     *
     * @param name  The name.
     * @param type  The type.
     *
     * @return  The texture.
     */
		GLuint get_texture(std::string name, aiTextureType type);

    /**
     * @fn  void TextureManager::Color4f(const aiColor4D *color);
     *
     * @brief Color 4f.
     *
     * @param color The color.
     */
		void Color4f(const aiColor4D *color);

    /**
     * @fn  void TextureManager::set_float4(float f[4], float a, float b, float c, float d);
     *
     * @brief Sets float 4.
     *
     * @param f The float to process.
     * @param a The float to process.
     * @param b The float to process.
     * @param c The float to process.
     * @param d The float to process.
     */
		void set_float4(float f[4], float a, float b, float c, float d);

    /**
     * @fn  void TextureManager::color4_to_float4(const aiColor4D *c, float f[4]);
     *
     * @brief Color 4 to float 4.
     *
     * @param c The const aiColor4D * to process.
     * @param f The float to process.
     */
		void color4_to_float4(const aiColor4D *c, float f[4]);

    /**
     * @fn  void TextureManager::apply_material(const aiMaterial *mtl );
     *
     * @brief Applies the material described by mtl.
     *
     * @param mtl The material.
     */
		void apply_material(const aiMaterial *mtl /*, std::map<std::string, GLuint*> &textureIdMap*/);

	private:
		std::map<std::string, std::map<aiTextureType, GLuint > > m_mTextures;
	};

}