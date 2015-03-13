#pragma once
#include <vector>
#include <map>

#include <assimp/material.h>


struct IXmlReader;

class Mesh;
class TextureManager;

struct aiScene;

namespace Assimp
{
	class Importer;
}


class Textured3DObject
{
public:
	Textured3DObject();
	~Textured3DObject();
	
	/// <summary>
	/// Load 3D objects from the specified path.
	/// </summary>
	/// <param name="path">The path</param>
	/// <param name="own_format">if set to <c>true</c> object is described in internal format (aogl),
	/// other type of formal otherwise</param>
	/// <returns></returns>
	bool load_object(std::string path, bool own_format, TextureManager * texmgr= nullptr);
	void bind_meshes();
	
	void set_name(std::string name) { m_sName = name; }
	std::string get_name(){ return m_sName; }

	void set_textures(const std::map< aiTextureType, GLuint >& textures, int mesh_index);
	GLuint get_texture(int mesh_index, int texture_index);

	const std::vector<Mesh *>& get_meshes() { return m_vMeshes; }

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
	/// <summary>
	/// Load object from file at the specified path.
	/// This method is called for format managed by assimp lib
	/// </summary>
	/// <param name="path">The path</param>
	/// <returns></returns>
	bool load_object(std::string path, TextureManager * texmgr);
	bool generate_meshes();
	bool generate_textures(TextureManager * texmgr);

protected:
	std::string m_sName;
	std::string m_sPath;
	Assimp::Importer *m_pImporter;
	const aiScene* m_pScene;

	std::vector<Mesh *	> m_vMeshes;
	// images / texture
	// map image filenames to textureIds
	// pointer to texture Array
	std::map<std::string, GLuint> textureIdMap;
	std::vector<std::string> m_vTexturePath;


	float m_fSize;
	float m_fSpacing;
	float m_fRange;
	float m_fSpeed;
	bool m_bRotating;
	bool m_bWasRotating;
	double m_dRotatingStartTime;
};

