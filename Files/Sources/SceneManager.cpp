#include "glew/glew.h"
#include "GLFW/glfw3.h"

#include <SceneManager.h>
#include <Camera.h>
#include <ProgramGUI.h>
#include <Light.h>
#include <DirectionalLight.h>
#include <ObjectManager.h>
#include <ShaderProgram.h>
#include <Textured3DObject.h>
#include <TextureManager.h>
#include <ShaderProgramManager.h>

#define M_PI 3.14

const float SceneManager::MOUSE_PAN_SPEED = 0.001f;
const float SceneManager::MOUSE_ZOOM_SPEED = 0.05f;
const float SceneManager::MOUSE_TURN_SPEED = 0.005f;

SceneManager::SceneManager()
	:m_iLockPositionX(0),
	m_iLockPositionY(0),
	m_bPanLock(false),
	m_bTurnLock(false),
	m_bZoomLock(false),
	m_pObjectManager(nullptr),
	m_pDeferredObjectManager(nullptr),
	m_iShadowMapSize(1024)
{
	m_pCamera = new Camera();
	m_pShaderProgramManager = new ShaderProgramManager();
}


SceneManager::~SceneManager()
{
	for each (Light* light in m_vLights)
	{
		delete light;
	}
	delete m_pCamera;
	delete m_pShaderProgramManager;
	delete m_pTextureManager;
	delete m_pObjectManager;
	delete m_pDeferredObjectManager;
}

void SceneManager::init()
{
	m_pCamera->Camera_defaults();
	m_bPanLock = false;
	m_bTurnLock = false;
	m_bZoomLock = false;
}

void SceneManager::setup_lights()
{
	Light *light = new Light();
	light->set_position(glm::vec3(0.0f, 1.0f, 0.0f));
	light->set_color(glm::vec3(0.15f, 0.15f, 0.15f));
	light->set_specular_power(30);
	m_vLights.push_back(light);

	m_pProgramGUI->add_gui_element("Light 1", light->generate_slider_position_infos("Light 1 Position"));
	m_pProgramGUI->add_gui_element("Light 1", light->generate_slider_color_infos("Light 1 Color"));
	m_pProgramGUI->add_gui_element("Light 1", light->generate_slider_specular_power_infos("Light 1 Specular Power"));

	/*light = new Light();
	light->set_position(glm::vec3(2.0f, 2.0f, 2.0f));
	light->set_color(glm::vec3(1.0f, 0.0f, 0.0f));
	light->set_specular_power(40.0f);
	m_vLights.push_back(light);

	m_pProgramGUI->add_gui_element("Light 2", light->generate_slider_position_infos("Light 2 Position"));
	m_pProgramGUI->add_gui_element("Light 2", light->generate_slider_color_infos("Light 2 Color"));
	m_pProgramGUI->add_gui_element("Light 2", light->generate_slider_specular_power_infos("Light 2 Specular Power"));
	*/
	auto dirlight = new DirectionalLight();
	dirlight->set_position(glm::vec3(1.0f, 1.0f, 0.0f));
	dirlight->set_direction(glm::vec3(-1.0f, -1.0f, 0.0f));
	dirlight->set_color(glm::vec3(0.0f, 1.0f, 0.0f));
	dirlight->set_specular_power(40.0f);
	m_vLights.push_back(dirlight);

	m_pProgramGUI->add_gui_element("Light 3", dirlight->generate_slider_position_infos("Light 3 Position"));
	m_pProgramGUI->add_gui_element("Light 3", dirlight->generate_slider_direction_infos("Light 3 Direction"));
	m_pProgramGUI->add_gui_element("Light 3", dirlight->generate_slider_color_infos("Light 3 Color"));
	m_pProgramGUI->add_gui_element("Light 3", dirlight->generate_slider_specular_power_infos("Light 3 Specular Power"));

}

void SceneManager::setup_shader_programs()
{
	m_pShaderProgramManager->create_main_shader_program();
	m_pShaderProgramManager->create_blit_shader_program();
	m_pShaderProgramManager->create_lighting_shader_program();
	m_pShaderProgramManager->create_pass_through_shader_program();
}

void SceneManager::setup_frame_buffer()
{
	// Create Framebuffer Object
	glGenFramebuffers(1, &m_gluiBufferFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gluiBufferFbo);

	// 2 draw buffers for color and normal
	GLuint gbufferDrawBuffers[2];
	// Initialize DrawBuffers
	gbufferDrawBuffers[0] = GL_COLOR_ATTACHMENT0;
	gbufferDrawBuffers[1] = GL_COLOR_ATTACHMENT1;
	glDrawBuffers(2, gbufferDrawBuffers);

	// Attach textures to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", COLOR), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", NORMAL), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", DEPTH), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Error on building framebuffer\n");
		exit(EXIT_FAILURE);
	}

	// Back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);





	glGenFramebuffers(1, &m_gluiShaderFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gluiShaderFbo);

	GLuint shadowRenderBuffer;
	glGenRenderbuffers(1, &shadowRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, shadowRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, m_iShadowMapSize, m_iShadowMapSize);
	// Attach the renderbuffer
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, shadowRenderBuffer);

	glGenTextures(1, &m_gluiShadowTexture);
	// Create shadow texture
	glBindTexture(GL_TEXTURE_2D, m_gluiShadowTexture);
	// Create empty texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_iShadowMapSize, m_iShadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	// Bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Color needs to be 0 outside of texture coordinates
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Attach the shadow texture to the depth attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_gluiShadowTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Error on building shadow framebuffer\n");
		exit(EXIT_FAILURE);
	}

	// Fall back to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneManager::setup_objects()
{
	m_pObjectManager = new ObjectManager(2);

	Textured3DObject*  cube = new Textured3DObject();
	cube->load_object_from_file(".\\Objects\\cube.aogl");
	int maxInstance = 10000;
	m_pObjectManager->bind_object(cube, maxInstance, 0);
	cube->set_textures(m_pTextureManager->get_textures("brick"));
	m_pProgramGUI->add_gui_element("Cube", m_pObjectManager->generate_slider_nb_instances_infos(0, maxInstance));
	m_pProgramGUI->add_gui_element("Cube", m_pObjectManager->generate_slider_cube_size(0));
	m_pProgramGUI->add_gui_element("Cube", m_pObjectManager->generate_slider_cube_radius_spacing(0));
	m_pProgramGUI->add_gui_element("Cube", m_pObjectManager->generate_slider_cube_range(0));
	m_pProgramGUI->add_gui_element("Cube", m_pObjectManager->generate_slider_cube_speed(0));
	m_pProgramGUI->add_gui_element("Cube", m_pObjectManager->generate_check_cube_rotation(0));

	Textured3DObject* plane = new Textured3DObject();;
	plane->load_object_from_file(".\\Objects\\plane.aogl");
	plane->set_textures(m_pTextureManager->get_textures("stone"));
	m_pObjectManager->bind_object(plane, 1, 1);

	m_pDeferredObjectManager = new ObjectManager(1);

	Textured3DObject* blit_plane = new Textured3DObject();;
	blit_plane->load_object_from_file(".\\Objects\\blit_plane.aogl");
	m_pDeferredObjectManager->bind_object(blit_plane, 1, 0);

	float numberOfInstance = 1.f;
}

void SceneManager::set_cam_states()
{
	if (m_pProgramGUI->get_left_button_state() == GLFW_PRESS)
		m_bTurnLock = true;
	else
		m_bTurnLock = false;

	if (m_pProgramGUI->get_right_button_state() == GLFW_PRESS)
		m_bZoomLock = true;
	else
		m_bZoomLock = false;

	if (m_pProgramGUI->get_middle_button_state() == GLFW_PRESS)
		m_bPanLock = true;
	else
		m_bPanLock = false;
}

void SceneManager::manage_camera_movements()
{
	// Cameras movements
	int altPressed = glfwGetKey(m_pProgramGUI->get_window(), GLFW_KEY_LEFT_SHIFT);
	if (!altPressed &&
		(m_pProgramGUI->get_left_button_state() == GLFW_PRESS ||
		m_pProgramGUI->get_right_button_state() == GLFW_PRESS ||
		m_pProgramGUI->get_middle_button_state() == GLFW_PRESS))
	{
		double x;
		double y;
		glfwGetCursorPos(m_pProgramGUI->get_window(), &x, &y);
		m_iLockPositionX = x;
		m_iLockPositionY = y;
	}
	if (altPressed == GLFW_PRESS)
	{
		double mousex;
		double mousey;
		glfwGetCursorPos(m_pProgramGUI->get_window(), &mousex, &mousey);
		int diffLockPositionX = mousex - m_iLockPositionX;
		int diffLockPositionY = mousey - m_iLockPositionY;
		if (m_bZoomLock)
		{
			float zoomDir = 0.0;
			if (diffLockPositionX > 0)
				zoomDir = -1.f;
			else
				if (diffLockPositionX < 0)
					zoomDir = 1.f;
			m_pCamera->Camera_zoom(zoomDir * MOUSE_ZOOM_SPEED);
		}
		else
		{
			if (m_bTurnLock)
			{
				m_pCamera->Camera_turn(diffLockPositionY * MOUSE_TURN_SPEED, diffLockPositionX * MOUSE_TURN_SPEED);
			}
			else
			{
				if (m_bPanLock)
				{
					m_pCamera->Camera_pan(diffLockPositionX * MOUSE_PAN_SPEED, diffLockPositionY * MOUSE_PAN_SPEED);
				}
			}
		}
		m_iLockPositionX = mousex;
		m_iLockPositionY = mousey;
	}
}

void SceneManager::display_scene(bool activate_fbo)
{
	
	//Default states
	glEnable(GL_DEPTH_TEST);
	shadow_map_management();
	
	if (activate_fbo)
	{
		// Bind gbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_gluiBufferFbo);
	}
	// Clear the front buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get camera matrices
	glm::mat4 projection = glm::perspective(45.0f, (float)m_pProgramGUI->get_width() / (float)m_pProgramGUI->get_height(), 0.1f, 100.f);
	glm::mat4 worldToView = glm::lookAt(m_pCamera->GetEye(), m_pCamera->GetO(), m_pCamera->GetUp());
	glm::mat4 objectToWorld;
	glm::mat4 mv = worldToView * objectToWorld;
	glm::mat4 mvp = projection * mv;
	glm::vec4 light = worldToView * glm::vec4(m_vLights.at(0)->get_position(), 1.0);


	// Select shader
	auto shader = m_pShaderProgramManager->get_shader(MAIN);
	if (shader != nullptr)
	{
		glUseProgram(shader->get_program());

		draw_scene(shader, mvp);

		// Fallback to default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Revert to window size viewport
		glViewport(0, 0, m_pProgramGUI->get_width(), m_pProgramGUI->get_height());

		if (activate_fbo)
		{
			debug_frame_buffer(mvp);
		}
	}
}

void SceneManager::draw_scene(ShaderProgram * shader, glm::mat4 mvp)
{
	// Upload uniforms
	shader->set_var_value("CamPos", glm::value_ptr(m_pCamera->GetEye()));
	shader->set_var_value("MVP", glm::value_ptr(mvp));
	shader->set_var_value("Time", (float)m_pProgramGUI->get_time());
	shader->set_var_value("Light1Pos", glm::value_ptr(m_vLights.at(0)->get_position()));
	shader->set_var_value("Light1Color", glm::value_ptr(m_vLights.at(0)->get_color()));
	shader->set_var_value("Light1SpecularPower", *(m_vLights.at(0)->get_specular_power()));

	/*	shader->set_var_value("Light2Pos", glm::value_ptr(m_vLights.at(1)->get_position()));
	shader->set_var_value("Light2Color", glm::value_ptr(m_vLights.at(1)->get_color()));
	shader->set_var_value("Light2SpecularPower", *(m_vLights.at(1)->get_specular_power()));
	*/
	auto baseLight = m_vLights.at(1);
	auto light = (DirectionalLight*)baseLight;
	shader->set_var_value("Light3Position", glm::value_ptr(light->get_position()));
	shader->set_var_value("Light3Direction", glm::value_ptr(light->get_direction()));
	shader->set_var_value("Light3Color", glm::value_ptr(light->get_color()));
	shader->set_var_value("Light3SpecularPower", *(light->get_specular_power()));

	for each (auto object in m_pObjectManager->get_objects())
	{
		if (object.first != nullptr)
		{
			glBindVertexArray(object.first->get_associated_vao());
			// Select textures
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, object.first->get_texture(DIFFUSE));
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, object.first->get_texture(SPECULAR));

			if (object.first->get_name() == "cube")
			{
				shader->set_var_value("isCube", (int)true);
				shader->set_var_value("Rotate", (int)*object.first->is_rotating());
				shader->set_var_value("InstanceNumber", *object.second);
				shader->set_var_value("SizeFactor", *object.first->get_size());
				shader->set_var_value("RadiusSpacing", *object.first->get_radius_spacing());
				shader->set_var_value("SpeedFactor", *object.first->get_speed());
				shader->set_var_value("RangeFactor", *object.first->get_range());
				auto cubeSize = 1 / (*object.first->get_size());
				auto nbCircles = *object.second / (cubeSize*M_PI);
				glDrawElementsInstanced(GL_TRIANGLES, object.first->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0, *object.second);
				shader->set_var_value("isCube", (int)false);
			}

			else
			{
				glDrawElements(GL_TRIANGLES, object.first->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
			}
		}
	}
}

void SceneManager::shadow_map_management()
{
	auto shader = m_pShaderProgramManager->get_shader(SHADOW_MAP);
	if (shader != nullptr)
	{
		glUseProgram(shader->get_program());
		//// Get camera matrices
		//glm::mat4 projection = glm::perspective(45.0f, (float)m_pProgramGUI->get_width() / (float)m_pProgramGUI->get_height(), 0.1f, 100.f);
		//glm::mat4 worldToView = glm::lookAt(m_pCamera->GetEye(), m_pCamera->GetO(), m_pCamera->GetUp());
		//glm::mat4 objectToWorld;
		//glm::mat4 mv = worldToView * objectToWorld;
		//glm::mat4 mvp = projection * mv;

		// Light space matrices
		// From light space to shadow map screen space
		glm::mat4 projection = glm::perspective(glm::radians(2.0f), 1.f, 1.f, 100.f);
		// From world to light
		auto baseLight = m_vLights.at(1);
		auto light = (DirectionalLight*)baseLight;
		glm::mat4 worldToLight = glm::lookAt(light->get_position(), light->get_position() + light->get_direction(), m_pCamera->GetUp()/* glm::vec3(0.f, 0.f, -1.f)*/);

		// From object to light (MV for light)
		glm::mat4 objectToWorld;
		glm::mat4 objectToLight = worldToLight * objectToWorld;
		// From object to shadow map screen space (MVP for light)
		glm::mat4 objectToLightScreen = projection * objectToLight;
		// From world to shadow map screen space 
		glm::mat4 worldToLightScreen = projection * worldToLight;

		// Bind the shadow FBO
		glBindFramebuffer(GL_FRAMEBUFFER, m_gluiShaderFbo);

		// Set the viewport corresponding to shadow texture resolution
		glViewport(0, 0, m_iShadowMapSize, m_iShadowMapSize);

		// Clear only the depth buffer
		glClear(GL_DEPTH_BUFFER_BIT);

		draw_scene(shader, objectToLightScreen);
	
		// Unbind the frambuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_pProgramGUI->get_width(), m_pProgramGUI->get_height());
	}

}


void SceneManager::debug_frame_buffer(glm::mat4 mvp)
{
	// Unbind the frambuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Render Debug information
	glDisable(GL_DEPTH_TEST);

	/*// Enable blending
	glEnable(GL_BLEND);
	// Setup additive blending
	glBlendFunc(GL_ONE, GL_ONE);*/

	//Render Lighting
	// Set a full screen viewport
	glViewport(0, 0, m_pProgramGUI->get_width(), m_pProgramGUI->get_height());
	// Use deferred point light shader
	auto shader = m_pShaderProgramManager->get_shader(LIGHT);
	if (shader != nullptr)
	{
		glUseProgram(shader->get_program());
		// Compute the inverse worldToScreen matrix
		glm::mat4 screenToWorld = glm::transpose(glm::inverse(mvp));

		shader->set_var_value("ScreenToWorld", glm::value_ptr(screenToWorld));
		shader->set_var_value("Light1Pos", glm::value_ptr(m_vLights.at(0)->get_position()));
		shader->set_var_value("Light1Color", glm::value_ptr(m_vLights.at(0)->get_color()));
		shader->set_var_value("CamPos", glm::value_ptr(m_pCamera->GetEye()));

		auto baseLight = m_vLights.at(1);
		auto light = (DirectionalLight*)baseLight;
		shader->set_var_value("Light3Position", glm::value_ptr(light->get_position()));
		shader->set_var_value("Light3Direction", glm::value_ptr(light->get_direction()));
		shader->set_var_value("Light3Color", glm::value_ptr(light->get_color()));
		shader->set_var_value("Light3SpecularPower", *(light->get_specular_power()));

		// Bind quad vao
		auto object = m_pDeferredObjectManager->get_object(0);
		glBindVertexArray(object.first->get_associated_vao());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", COLOR));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", NORMAL));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", DEPTH));

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_gluiShadowTexture);
		
		// Render quad
		glDrawElements(GL_TRIANGLES, object.first->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);

		auto blitShader = m_pShaderProgramManager->get_shader(BLIT);
		if (blitShader != nullptr)
		{
			// Use the blit program
			glUseProgram(blitShader->get_program());
			// Bind quad VAO
			glBindVertexArray(object.first->get_associated_vao());
			int smallWidth = m_pProgramGUI->get_width() / 4;
			int smallHeight = m_pProgramGUI->get_height() / 4;

			glActiveTexture(GL_TEXTURE0);
			glViewport(0, 0, smallWidth, smallHeight);
			// Bind gbuffer color texture
			glBindTexture(GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", COLOR));
			// Draw quad
			glDrawElements(GL_TRIANGLES, object.first->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);


			glViewport(m_pProgramGUI->get_width() / 4, 0, smallWidth, smallHeight);
			// Bind gbuffer color texture
			glBindTexture(GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", NORMAL));
			// Draw quad
			glDrawElements(GL_TRIANGLES, object.first->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);


			glViewport(2 * m_pProgramGUI->get_width() / 4, 0, smallWidth, smallHeight);
			// Bind gbuffer color texture
			glBindTexture(GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", DEPTH));
			// Draw quad
			glDrawElements(GL_TRIANGLES, object.first->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);


			glBindTexture(GL_TEXTURE_2D, m_gluiShadowTexture);

			glViewport(3 * m_pProgramGUI->get_width() / 4, 0, smallWidth, smallHeight);
			// Draw quad
			glDrawElements(GL_TRIANGLES, object.first->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);


		}
	}
	// Disable blending
	//	glDisable(GL_BLEND);

}
