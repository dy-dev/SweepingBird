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


struct IXmlReader;


struct aiScene;

namespace Assimp
{
	class Importer;
}

namespace SweepingBirds
{
	enum ObjectType
	{
		GROUND,
		BIRD,
		BAT,
		SKYBOX
	};

	class Mesh;
	class TextureManager;
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
		bool load_object(std::string path, bool own_format, TextureManager * texmgr = nullptr);
		void bind_meshes();


		void set_name(std::string name) { m_sName = name; }
		std::string get_name(){ return m_sName; }

		void set_object_type(ObjectType name) { m_eType = name; }
		ObjectType get_object_type(){ return m_eType; }
		

		void set_textures(const std::map< aiTextureType, GLuint >& textures, int mesh_index);
		GLuint get_texture(int mesh_index, int texture_index);

		const std::vector<Mesh *>& get_meshes() { return m_vMeshes; }

		float * get_size() { return &m_fSize; }
		void set_size(float size) { m_fSize = size; }

		float * get_height() { return &m_fHeight; }
		void set_height(float height) { m_fHeight = height; }


		void set_position(glm::vec3 Position) { m_v3Position = Position; };
		glm::vec3 get_position(){ return m_v3Position; };

		void set_direction(glm::vec3 Direction) { m_v3Direction = Direction; };
		glm::vec3 get_direction(){ return m_v3Direction; };

		void set_rotation_angle(float angle) { m_fRotationAngle = angle; }
		float get_rotation_angle() { return m_fRotationAngle; }

		
		float * get_x_pos() { return &(m_v3Position.x); }
		float * get_y_pos() { return &(m_v3Position.y); }
		float * get_z_pos() { return &(m_v3Position.z); }

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
		ObjectType m_eType;
		Assimp::Importer *m_pImporter;
		const aiScene* m_pScene;

		std::vector<Mesh *	> m_vMeshes;
		// images / texture
		// map image filenames to textureIds
		// pointer to texture Array
		std::map<std::string, GLuint> textureIdMap;
		std::vector<std::pair<std::string, aiTextureType> > m_vTexturePath;

		glm::vec3 m_v3Position;
		glm::vec3 m_v3Direction;

		float m_fSize;
		float m_fHeight;
		float m_fSpacing;
		float m_fRange;
		float m_fSpeed;
		float m_fRotationAngle;
		bool m_bRotating;
		bool m_bWasRotating;
		double m_dRotatingStartTime;
	};
}
