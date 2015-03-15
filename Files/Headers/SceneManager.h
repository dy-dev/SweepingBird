#pragma once
#include <vector>

#include <glm/mat4x4.hpp> // glm::mat4

#include <ShaderProgram.h>
#include <ogldev_math_3d.h>

class SkyBox;
class OVGCamera;
namespace SweepingBirds
{
	class Camera;
	class ProgramGUI;
	class Light;
	class ObjectManager;
	class TextureManager;
	class ShaderProgramManager;


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
		bool setup_skybox();

		void draw_scene(ShaderProgram * shader, glm::mat4 mvp, glm::mat4 mv);
		void gamma_management(glm::mat4 mvp, glm::mat4 mv);
		void debug_frame_buffer(glm::mat4 mvp, glm::mat4 mv, bool activate_shadow_map, bool activate_sobel_map);

		Camera * get_camera() const { return m_pCamera; }

		static void demo(SceneManager * scn, double start);
		void setupdemo();

		void updateBird(const glm::vec3& birdPosition, float birdAngle);
		void updatePredators(const std::vector<glm::vec3>& predatorsPositions, const std::vector<glm::vec3>& predatorsDirections);

	protected:
		Camera * m_pCamera;
		ProgramGUI * m_pProgramGUI;
		std::vector<Light*> m_vLights;
		OVGCamera* m_pGameCamera;
		PersProjInfo m_persProjInfo;
		SkyBox * m_pSkyBox;
		ShaderProgramManager* m_pShaderProgramManager;
		ObjectManager* m_pAssimpObjectManager;
		TextureManager * m_pTextureManager;

		int m_iLockPositionX;
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