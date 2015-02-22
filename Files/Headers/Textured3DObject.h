#pragma once
#include <string>
#include <Windows.h>
#include <vector>

enum AttributeType
{
	NONE,
	TRIANGLES,
	UVS,
	VERTICES,
	NORMALS
};

class IXmlReader;
class Textured3DObject
{
public:
	Textured3DObject();
	~Textured3DObject();

	void load_object_from_file(std::string path);
	
	void set_associated_vao(GLuint vao) { m_iVAO = vao;}
	GLuint get_associated_vao(){ return m_iVAO; };

	void set_textures(std::vector<GLuint> texture) { m_vTexture = texture; }
	GLuint get_texture(int index);

	void set_name(std::string name) {m_sName = name;}
	std::string get_name(){ return m_sName; }
	std::vector<int> get_trianglesList() const { return m_vTrianglesList; };
	int  get_triangles_count() { return m_iTrianglesCount; }
	int  get_dimension() { return m_iDimension; }
	std::vector<float > get_uvs() const { return m_vUVs; };
	std::vector<float > get_vertices() const { return m_vVertices; };
	std::vector<float > get_normals() const { return m_vNormals; };


	float * get_size() { return &m_fSize; }
	void set_size(float size) { m_fSize = size; }
	void set_radius_spacing(float spacing) { m_fSpacing = spacing; }
	float * get_radius_spacing() { return &m_fSpacing; }
	void set_range(float range) { m_fRange = range; }
	float * get_range() { return &m_fRange; }
	void  set_speed(float speed) { m_fSpeed = speed; }
	float * get_speed() { return &m_fSpeed; }
	bool * is_rotating(){ return &m_bRotating; }
	bool check_start_rotation();
	void set_rotating(bool rot){ m_bRotating = rot; }
	void set_rotating_start(double start){ m_dRotatingStartTime = start; }
	double get_rotating_start(){ return m_dRotatingStartTime; }
private:
	HRESULT get_attributes(IXmlReader* pReader);

	std::string m_sName;
	GLuint m_iVAO;
	std::vector<GLuint> m_vTexture;

	AttributeType m_eCurrentAttribute;
	int m_iTrianglesCount;
	int m_iDimension;
	std::vector<int> m_vTrianglesList;
	std::vector<float> m_vUVs;
	std::vector<float> m_vVertices;
	std::vector<float> m_vNormals;

	float m_fSize;
	float m_fSpacing;
	float m_fRange;
	float m_fSpeed;
	bool m_bRotating;
	bool m_bWasRotating;
	double m_dRotatingStartTime;
};		 

