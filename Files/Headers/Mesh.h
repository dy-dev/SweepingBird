#pragma once
#include <string>
#include <Windows.h>
#include <map>
#include <vector>

#include <material.h>

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2
#define FRAG_COLOR	0

/**
 * @enum  AttributeType
 *
 * @brief Values that represent attribute types.
 */
enum AttributeType
{
	NONE,
	TRIANGLES,
	UVS,
	VERTICES,
	NORMALS
};

struct IXmlReader;
struct aiMesh;

namespace SweepingBirds
{
	// This is for a shader uniform block
	struct MyMaterial{

		float diffuse[4];
		float ambient[4];
		float specular[4];
		float emissive[4];
		float shininess;
		int texCount;
	};

	
	class Mesh
	{
	public:
    /**
     * @fn  Mesh::Mesh();
     *
     * @brief Default constructor.
     */
		Mesh();
		~Mesh();

    /**
     * @fn  bool Mesh::load_mesh_from_file(std::string path, std::string &name);
     *
     * @brief Loads mesh from file.
     *
     * @param path            Full pathname of the file.
     * @param [in,out]  name  The name.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool load_mesh_from_file(std::string path, std::string &name);

    /**
     * @fn  bool Mesh::fill_vertices_infos(std::string name, const aiMesh* assimpMesh);
     *
     * @brief Fill vertices infos.
     *
     * @param name        The name.
     * @param assimpMesh  The assimp mesh.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool fill_vertices_infos(std::string name, const aiMesh* assimpMesh);

    /**
     * @fn  void Mesh::set_material(aiMaterial * mat)
     *
     * @brief Sets a material.
     *
     * @param [in,out]  mat If non-null, the matrix.
     */
		void set_material(aiMaterial * mat) { m_pAiMaterial = mat; }

    /**
     * @fn  aiMaterial * Mesh::get_material()
     *
     * @brief Gets the material.
     *
     * @return  null if it fails, else the material.
     */
		aiMaterial * get_material() { return m_pAiMaterial; }

    /**
     * @fn  GLuint Mesh::get_vao()
     *
     * @brief Gets the vao.
     *
     * @return  The vao.
     */
		GLuint get_vao(){ return m_iVAO; };

    /**
     * @fn  void Mesh::bind();
     *
     * @brief Binds this object.
     */
		void bind();

    /**
     * @fn  void Mesh::set_textures(const std::map< aiTextureType, GLuint >& texture)
     *
     * @brief Sets the textures.
     *
     * @param texture The texture.
     */
		void set_textures(const std::map< aiTextureType, GLuint >& texture) { m_mTexture = texture; }

    /**
     * @fn  GLuint Mesh::get_texture(aiTextureType type);
     *
     * @brief Gets a texture.
     *
     * @param type  The type.
     *
     * @return  The texture.
     */
		GLuint get_texture(aiTextureType type);

    /**
     * @fn  std::vector<int> Mesh::get_trianglesList() const
     *
     * @brief Gets triangles list.
     *
     * @return  The triangles list.
     */
		const std::vector<int>& get_trianglesList() const { return m_vTrianglesList; };

    /**
     * @fn  int Mesh::get_triangles_count()
     *
     * @brief Gets triangles count.
     *
     * @return  The triangles count.
     */
		int  get_triangles_count() { return m_iTrianglesCount; }

    /**
     * @fn  int Mesh::get_dimension()
     *
     * @brief Gets the dimension.
     *
     * @return  The dimension.
     */
		int  get_dimension() { return m_iDimension; }

    /**
     * @fn  std::vector<float > Mesh::get_uvs() const
     *
     * @brief Gets the uvs.
     *
     * @return  The uvs.
     */
		const std::vector<float >& get_uvs() const { return m_vUVs; };

    /**
     * @fn  std::vector<float > Mesh::get_vertices() const
     *
     * @brief Gets the vertices.
     *
     * @return  The vertices.
     */
		const std::vector<float >& get_vertices() const { return m_vVertices; };

    /**
     * @fn  std::vector<float > Mesh::get_normals() const
     *
     * @brief Gets the normals.
     *
     * @return  The normals.
     */
		const std::vector<float >& get_normals() const { return m_vNormals; };

		void set_normals(std::vector<float >& norms);

	private:
    /**
     * @fn  HRESULT Mesh::get_attributes(IXmlReader* pReader, std::string &name);
     *
     * @brief Gets the attributes.
     *
     * @param [in,out]  pReader If non-null, the reader.
     * @param [in,out]  name    The name.
     *
     * @return  The attributes.
     */
		HRESULT get_attributes(IXmlReader* pReader, std::string &name);

	private:
		GLuint m_iVAO;
		GLuint m_iVBO[4];
		std::string m_sName;
		std::map< aiTextureType, GLuint > m_mTexture;
		aiMaterial * m_pAiMaterial;

		AttributeType m_eCurrentAttribute;
		int m_iTrianglesCount;
		int m_iDimension;
		std::vector<int> m_vTrianglesList;
		std::vector<float> m_vUVs;
		std::vector<float> m_vVertices;
		std::vector<float> m_vNormals;
	};

}