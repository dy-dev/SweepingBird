#pragma once
#include <vector>

#include "glm/mat4x4.hpp" // glm::mat4

#include <ShaderProgram.h>

class GLFWwindow;
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
	void display_scene(bool activate_fbo);

	void set_programGUI(ProgramGUI * programGUI){ m_pProgramGUI = programGUI; }
	void set_texture_manager(TextureManager* textureManager){ m_pTextureManager = textureManager; }
	void set_cam_states();
	void setup_lights();
	void setup_shader_programs();
	void setup_frame_buffer();
	void setup_objects();

	void draw_scene(ShaderProgram * shader, glm::mat4 mvp);
	void shadow_map_management();
	void debug_frame_buffer(glm::mat4 mvp);
	void debug_shadow_frame_buffer(glm::mat4 mvp);
	
	Camera * get_camera() const { return m_pCamera; }

	int get_shadow_texture_size(){ return m_iShadowMapSize; }
protected:
	Camera * m_pCamera;
	ProgramGUI * m_pProgramGUI;
	std::vector<Light*> m_vLights;
	
	ShaderProgramManager* m_pShaderProgramManager;
	ObjectManager* m_pObjectManager;
	ObjectManager* m_pDeferredObjectManager;
	TextureManager * m_pTextureManager;
	int m_iLockPositionX;
	int m_iLockPositionY;

	bool m_bPanLock;
	bool m_bTurnLock;
	bool m_bZoomLock;

	GLuint m_gluiBufferFbo;
	GLuint m_gluiShaderFbo;
	GLuint m_gluiShadowTexture;

	int m_iShadowMapSize;
	
	static const float MOUSE_PAN_SPEED;
	static const float MOUSE_ZOOM_SPEED;
	static const float MOUSE_TURN_SPEED;

};

