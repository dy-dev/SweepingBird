#pragma once
#include <string>
#include <Windows.h>
#include <map>
#include <vector>


#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2
#define FRAG_COLOR	0

enum AttributeType
{
	NONE,
	TRIANGLES,
	UVS,
	VERTICES,
	NORMALS
};

struct IXmlReader;


// This is for a shader uniform block
struct MyMaterial{

	float diffuse[4];
	float ambient[4];
	float specular[4];
	float emissive[4];
	float shininess;
	int texCount;
};


struct aiMesh;

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool load_mesh_from_file(std::string path, std::string &name);
	bool fill_vertices_infos(std::string name, const aiMesh* assimpMesh);
	
	void set_material(aiMaterial * mat) { m_pAiMaterial = mat; }
	aiMaterial * get_material() { return m_pAiMaterial; }
	GLuint get_vao(){ return m_iVAO; };

	void bind();

	void set_textures(const std::map< aiTextureType, GLuint >& texture) { m_mTexture = texture; }
	GLuint get_texture(aiTextureType type);

	std::vector<int> get_trianglesList() const { return m_vTrianglesList; };
	int  get_triangles_count() { return m_iTrianglesCount; }
	int  get_dimension() { return m_iDimension; }
	std::vector<float > get_uvs() const { return m_vUVs; };
	std::vector<float > get_vertices() const { return m_vVertices; };
	std::vector<float > get_normals() const { return m_vNormals; };

private :
	HRESULT get_attributes(IXmlReader* pReader, std::string &name);
	
private:
	GLuint m_iVAO;
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

