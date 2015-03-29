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
  //Helper allowing to retrieve the class name
  //of an object
  template<typename T>
  struct ClassName
  {
    std::string Name()
    {
      return typeid(T).name();
    }
  };

	class Mesh;
	class TextureManager;
	class ProgramGUI;
	class ShaderProgram;
	class ShaderProgramManager;
	class ObjectManager;
	enum ShaderProgramType;
	class Camera;
	class Textured3DObject
	{
	public:
    /**
     * @fn  static void Textured3DObject::jump_cam(void *obj, bool stick = false);
     *
     * @brief Jump camera.
     *
     * @param [in,out]  obj If non-null, the object.
     * @param stick         true to stick.
     */
		static void jump_cam(void *obj, bool stick = false);

    /**
     * @fn  static void Textured3DObject::stick_cam(void *obj);
     *
     * @brief Stick camera.
     *
     * @param [in,out]  obj If non-null, the object.
     */
		static void stick_cam(void *obj);

	public:
    /**
     * @fn  Textured3DObject::Textured3DObject();
     *
     * @brief Default constructor.
     */
		Textured3DObject();

    /**
     * @fn  Textured3DObject::Textured3DObject(TextureManager* texMgr);
     *
     * @brief Constructor.
     *
     * @param [in,out]  texMgr  If non-null, manager for tex.
     */
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

    /**
     * @fn  void Textured3DObject::bind_meshes();
     *
     * @brief Bind meshes.
     */
		void bind_meshes();

    /**
     * @fn  void Textured3DObject::set_name(std::string name)
     *
     * @brief Sets a name.
     *
     * @param name  The name.
     */
		void set_name(std::string name) { m_sName = name; }

    /**
     * @fn  std::string Textured3DObject::get_name()
     *
     * @brief Gets the name.
     *
     * @return  The name.
     */
		std::string get_name(){ return m_sName; }

    /**
     * @fn  void Textured3DObject::set_textures(const std::map< aiTextureType, GLuint >& textures, int mesh_index);
     *
     * @brief Sets the textures.
     *
     * @param textures    The textures.
     * @param mesh_index  Zero-based index of the mesh.
     */
		void set_textures(const std::map< aiTextureType, GLuint >& textures, int mesh_index);

    /**
     * @fn  GLuint Textured3DObject::get_texture(int mesh_index, int texture_index);
     *
     * @brief Gets a texture.
     *
     * @param mesh_index    Zero-based index of the mesh.
     * @param texture_index Zero-based index of the texture.
     *
     * @return  The texture.
     */
		GLuint get_texture(int mesh_index, int texture_index);

    /**
     * @fn  const std::vector<Mesh *>& Textured3DObject::get_meshes()
     *
     * @brief Gets the meshes.
     *
     * @return  null if it fails, else the meshes.
     */
		const std::vector<Mesh *>& get_meshes() { return m_vMeshes; }

    /**
     * @fn  float * Textured3DObject::get_size()
     *
     * @brief Gets the size.
     *
     * @return  null if it fails, else the size.
     */
		float * get_size() { return &m_fSize; }

    /**
     * @fn  void Textured3DObject::set_size(float size)
     *
     * @brief Sets a size.
     *
     * @param size  The size.
     */
		void set_size(float size) { m_fSize = size; }

    /**
     * @fn  void Textured3DObject::set_position(glm::vec3 Position)
     *
     * @brief Sets a position.
     *
     * @param Position  The position.
     */
		void set_position(glm::vec3 Position) { m_v3PrevPos = m_v3Position;  m_v3Position = Position; };

    /**
     * @fn  glm::vec3 Textured3DObject::get_position()
     *
     * @brief Gets the position.
     *
     * @return  The position.
     */
		glm::vec3 get_position(){ return m_v3Position; };

    /**
     * @fn  glm::vec3 Textured3DObject::get_translation()
     *
     * @brief Gets the translation.
     *
     * @return  The translation.
     */
		glm::vec3 get_translation(){ return m_v3Position - m_v3PrevPos; };

    /**
     * @fn  void Textured3DObject::set_direction(glm::vec3 direction)
     *
     * @brief Sets a direction.
     *
     * @param direction The direction.
     */
		void set_direction(glm::vec3 direction) { m_v3Direction = direction;};

    /**
     * @fn  glm::vec3 Textured3DObject::get_direction()
     *
     * @brief Gets the direction.
     *
     * @return  The direction.
     */
		glm::vec3 get_direction(){ return m_v3Direction; };

    /**
     * @fn  float * Textured3DObject::get_x_pos()
     *
     * @brief Get x coordinate position.
     *
     * @return  null if it fails, else the x coordinate position.
     */
		float * get_x_pos() { return &(m_v3Position.x); }

    /**
     * @fn  float * Textured3DObject::get_y_pos()
     *
     * @brief Get y coordinate position.
     *
     * @return  null if it fails, else the y coordinate position.
     */
		float * get_y_pos() { return &(m_v3Position.y); }

    /**
     * @fn  float * Textured3DObject::get_z_pos()
     *
     * @brief Get z coordinate position.
     *
     * @return  null if it fails, else the z coordinate position.
     */
		float * get_z_pos() { return &(m_v3Position.z); }

    /**
     * @fn  const float * Textured3DObject::get_y_pos() const
     *
     * @brief Get y coordinate position.
     *
     * @return  null if it fails, else the y coordinate position.
     */
		const float * get_y_pos() const { return &(m_v3Position.y); }

		/**
		* @fn  virtual float * Textured3DObject::get_rot_angle()
		*
		* @brief Gets rot angle.
		*
		* @return  null if it fails, else the rot angle.
		*/
		virtual float * get_rot_angle() { return &m_fRotAngle; }

		/**
		* @fn  virtual float * Textured3DObject::get_zoom()
		*
		* @brief Gets the zoom on the object
		*
		* @return  null if it fails, else the zoom
		*/
		virtual float * get_zoom() { return &m_fZoom; }
		
		virtual bool is_cam_focused() { return m_bCamFocused; }
		
		/**
     * @fn  void Textured3DObject::set_mock_pos(const glm::vec3& newPos)
     *
     * @brief Sets mock position.
     *
     * @param newPos  The new position.
     */
		void set_mock_pos(const glm::vec3& newPos){m_v3MockPos = newPos;}

    /**
     * @fn  const glm::vec3& Textured3DObject::get_mock_pos() const
     *
     * @brief Gets mock position.
     *
     * @return  The mock position.
     */
		const glm::vec3& get_mock_pos() const{ return m_v3MockPos; }

    /**
     * @fn  virtual void Textured3DObject::draw(ShaderProgramManager& shader, glm::mat4 proj, float time, int nbInstance) = 0;
     *
     * @brief Draws.
     *
     * @param [in,out]  shader  The shader.
     * @param proj              The project.
     * @param time              The time.
     * @param nbInstance        The nb instance.
     */
		virtual void draw(ShaderProgramManager& shader, glm::mat4 proj, float time, int nbInstance) = 0;

    /**
     * @fn  ShaderProgram* Textured3DObject::setup_drawing_space(ShaderProgramManager& shader, Mesh* mesh, glm::mat4 proj, float time);
     *
     * @brief Sets up the drawing space.
     *
     * @param [in,out]  shader  The shader.
     * @param [in,out]  mesh    If non-null, the mesh.
     * @param proj              The project.
     * @param time              The time.
     *
     * @return  null if it fails, else a ShaderProgram*.
     */
		ShaderProgram* setup_drawing_space(ShaderProgramManager& shader, Mesh* mesh, glm::mat4 proj, float time);

    /**
     * @fn  void Textured3DObject::clean_bindings();
     *
     * @brief Clean bindings.
     */
		void clean_bindings();

	private:
		/// <summary>
		/// Load object from file at the specified path.
		/// This method is called for format managed by assimp lib
		/// </summary>
		/// <param name="path">The path</param>
		/// <returns></returns>
		bool load_object(std::string path, TextureManager * texmgr);

    /**
     * @fn  bool Textured3DObject::generate_meshes();
     *
     * @brief Generates the meshes.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool generate_meshes();

    /**
     * @fn  bool Textured3DObject::generate_textures(TextureManager * texmgr);
     *
     * @brief Generates the textures.
     *
     * @param [in,out]  texmgr  If non-null, the texmgr.
     *
     * @return  true if it succeeds, false if it fails.
     */
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

		bool m_bCamSticked;
		bool m_bCamFocused;
		float m_fRotAngle;
		float m_fZoom;

		/**
		 *	
		 */
		TextureManager * m_pTextureManager;
		ObjectManager* m_pObjectManager;

		ShaderProgramType m_eShaderType;
	};
}
