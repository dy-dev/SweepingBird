#pragma once
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <assimp/material.h>
#include <glew/glew.h>

struct IXmlReader;
struct aiScene;

namespace Assimp
{
	class Importer;
}

namespace SweepingBirds
{
	class Mesh;
	class TextureManager;
	class ProgramGUI;
	class Textured3DObject
	{
	public:
		Textured3DObject();
		Textured3DObject(TextureManager* texMgr);
		
		~Textured3DObject();

		/// <summary>
		/// Load 3D objects from the specified path.
		/// </summary>
		/// <param name="path">The path</param>
		/// <param name="own_format">if set to <c>true</c> object is described in internal format (aogl),
		/// other type of formal otherwise</param>
		/// <returns></returns>
		bool load_object(std::string path, bool own_format, TextureManager * texmgr = nullptr);
		void bind_meshes();

		void set_name(std::string name) { m_sName = name; }
		std::string get_name(){ return m_sName; }

		void set_textures(const std::map< aiTextureType, GLuint >& textures, int mesh_index);
		GLuint get_texture(int mesh_index, int texture_index);

		const std::vector<Mesh *>& get_meshes() { return m_vMeshes; }

		float * get_size() { return &m_fSize; }
		void set_size(float size) { m_fSize = size; }

		void set_position(glm::vec3 Position) { m_v3PrevPos = m_v3Position;  m_v3Position = Position; };
		glm::vec3 get_position(){ return m_v3Position; };
		glm::vec3 get_translation(){ return m_v3Position - m_v3PrevPos; };
		
		float * get_x_pos() { return &(m_v3Position.x); }
		float * get_y_pos() { return &(m_v3Position.y); }
		float * get_z_pos() { return &(m_v3Position.z); }
		const float * get_y_pos() const { return &(m_v3Position.y); }


		void set_mock_pos(const glm::vec3& newPos){m_v3MockPos = newPos;}
		const glm::vec3& get_mock_pos() const{ return m_v3MockPos; }

		virtual void draw() = 0;

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
	
		/**
		 *	Assimp Inputs
		 */
		Assimp::Importer *m_pImporter;
		const aiScene* m_pScene;
		std::vector<Mesh *	> m_vMeshes;
		std::map<std::string, GLuint> textureIdMap; // map image filenames to textureIds
		std::vector<std::pair<std::string, aiTextureType> > m_vTexturePath;

		/**
		 *	3D datas
		 */
		glm::vec3 m_v3Position;
		glm::vec3 m_v3Direction;
		glm::vec3 m_v3PrevPos;
		glm::vec3 m_v3MockPos;

		float m_fSize;

		/**
		 *	
		 */
		TextureManager * m_pTextureManager;
		ProgramGUI * m_pProgramGUI;
	};
}
