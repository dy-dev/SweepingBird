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
		SceneManager();
		~SceneManager();

		void init();
		void manage_camera_movements();
		void display_scene(bool activate_gamma);

		void set_programGUI(ProgramGUI * programGUI){ m_pProgramGUI = programGUI; }
		void set_texture_manager(TextureManager* textureManager){ m_pTextureManager = textureManager; }
		void set_cam_states();
		void setup_lights();
		void setup_shader_programs();
		void setup_frame_buffer();
		bool setup_objects();

		void set_time(double time){ m_dTime = time; }
		double get_time(){ return m_dTime; }

		void gamma_management(glm::mat4 mvp, glm::mat4 mv);
		void debug_frame_buffer(glm::mat4 mvp, glm::mat4 mv, bool activate_shadow_map, bool activate_sobel_map);

		Camera * get_camera() const { return m_pCamera; }

		static void demo(SceneManager * scn, double start);
		void setupdemo();

		double get_framerate(){ return m_dFPS; }
		void update_time(double newTime);

		ObjectManager& get_object_manager() const { assert(m_pAssimpObjectManager); return *m_pAssimpObjectManager; }
		const std::vector<Light*>& get_lights() {return m_vLights;}

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