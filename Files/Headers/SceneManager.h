#pragma once
#include <vector>

#include <glm/mat4x4.hpp> // glm::mat4

#include <ShaderProgram.h>
#include <ogldev_math_3d.h>
#include <GPUBuffer.h>

namespace SweepingBirds
{
	class Camera;
	class ProgramGUI;
	class Light;
	class ObjectManager;
	class TextureManager;
	class ShaderProgramManager;
	class Textured3DObject;

	class SceneManager
	{
	public:
    /**
     * @fn  SceneManager::SceneManager();
     *
     * @brief Default constructor.
     */
		SceneManager();
		~SceneManager();

    /**
     * @fn  void SceneManager::init();
     *
     * @brief Initialises this object.
     */
		void init();

    /**
     * @fn  void SceneManager::manage_camera_movements();
     *
     * @brief Manage camera movements.
     */
		void manage_camera_movements();

    /**
     * @fn  void SceneManager::display_scene(bool activate_gamma);
     *
     * @brief Displays a scene described by activate_gamma.
     *
     * @param activate_gamma  true to activate gamma.
     */
		void display_scene(bool activate_gamma);

    /**
     * @fn  void SceneManager::set_programGUI(ProgramGUI * programGUI)
     *
     * @brief Sets program graphical user interface.
     *
     * @param [in,out]  programGUI  If non-null, the program graphical user interface.
     */
		void set_programGUI(ProgramGUI * programGUI){ m_pProgramGUI = programGUI; }

    /**
     * @fn  void SceneManager::set_texture_manager(TextureManager* textureManager)
     *
     * @brief Sets texture manager.
     *
     * @param [in,out]  textureManager  If non-null, manager for texture.
     */
		void set_texture_manager(TextureManager* textureManager){ m_pTextureManager = textureManager; }

    /**
     * @fn  void SceneManager::set_cam_states();
     *
     * @brief Sets camera states.
     */
		void set_cam_states();

    /**
     * @fn  void SceneManager::setup_lights();
     *
     * @brief Sets up the lights.
     */
		void setup_lights();

    /**
     * @fn  void SceneManager::setup_shader_programs();
     *
     * @brief Sets up the shader programs.
     */
		void setup_shader_programs();

    /**
     * @fn  void SceneManager::setup_frame_buffer();
     *
     * @brief Sets up the frame buffer.
     */
		void setup_frame_buffer();

    /**
     * @fn  bool SceneManager::setup_objects();
     *
     * @brief Sets up the objects.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool setup_objects();

    /**
     * @fn  void SceneManager::set_time(double time)
     *
     * @brief Sets a time.
     *
     * @param time  The time.
     */
		void set_time(double time){ m_dTime = time; }

    /**
     * @fn  double SceneManager::get_time()
     *
     * @brief Gets the time.
     *
     * @return  The time.
     */
		double get_time(){ return m_dTime; }

    /**
     * @fn  void SceneManager::gamma_management(glm::mat4 mvp, glm::mat4 mv);
     *
     * @brief Gamma management.
     *
     * @param mvp The mvp.
     * @param mv  The mv.
     */
		void gamma_management(glm::mat4 mvp, glm::mat4 mv);

    /**
     * @fn  void SceneManager::debug_frame_buffer(glm::mat4 mvp, glm::mat4 mv, bool activate_shadow_map, bool activate_sobel_map);
     *
     * @brief Debug frame buffer.
     *
     * @param mvp                 The mvp.
     * @param mv                  The mv.
     * @param activate_shadow_map true to activate shadow map.
     * @param activate_sobel_map  true to activate sobel map.
     */
		void debug_frame_buffer(glm::mat4 mvp, glm::mat4 mv, bool activate_shadow_map, bool activate_sobel_map);

    /**
     * @fn  Camera * SceneManager::get_camera() const
     *
     * @brief Gets the camera.
     *
     * @return  null if it fails, else the camera.
     */
		Camera * get_camera() const { return m_pCamera; }

    /**
     * @fn  static void SceneManager::demo(SceneManager * scn, double start);
     *
     * @brief Demoes.
     *
     * @param [in,out]  scn If non-null, the scn.
     * @param start         The start.
     */
		static void demo(SceneManager * scn, double start);

    /**
     * @fn  void SceneManager::setupdemo();
     *
     * @brief Setupdemoes this object.
     */
		void setupdemo();

    /**
     * @fn  double SceneManager::get_framerate()
     *
     * @brief Gets the framerate.
     *
     * @return  The framerate.
     */
		double get_framerate(){ return m_dFPS; }

    /**
     * @fn  void SceneManager::update_time(double newTime);
     *
     * @brief Updates the time described by newTime.
     *
     * @param newTime The new time.
     */
		void update_time(double newTime);

    /**
     * @fn  ObjectManager& SceneManager::get_object_manager() const
     *
     * @brief Gets object manager.
     *
     * @return  The object manager.
     */
    ObjectManager& get_object_manager() const { assert(m_pAssimpObjectManager); return *m_pAssimpObjectManager; }

	protected:
		ProgramGUI * m_pProgramGUI;
		Camera * m_pCamera;
		ShaderProgramManager* m_pShaderProgramManager;
		TextureManager * m_pTextureManager;

		ObjectManager* m_pAssimpObjectManager;
		std::vector<Light*> m_vLights;

		GLsizei m_siNbPredators;

		double m_dFPS;
		double m_dTime;
		double m_dPrevTime;
		
		int m_iLockPositionX;
		int m_iLockTurnPositionX;
		int m_iLockPositionY;

		bool m_bPanLock;
		bool m_bTurnLock;
		bool m_bZoomLock;

		GLuint m_gluiBufferFbo;
		GLuint m_gluiShaderFbo;

		GLuint m_gluiFxFbo;
		GLuint * m_agluiFxTextures;

		float m_fGamma;

		static const float MOUSE_PAN_SPEED;
		static const float MOUSE_ZOOM_SPEED;
		static const float MOUSE_TURN_SPEED;

		double m_dStartTime;
		bool m_bDemoRunning;
	};

}