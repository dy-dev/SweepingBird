#include <windows.h>

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <SceneManager.h>
#include <Camera.h>
#include <ProgramGUI.h>
#include <Light.h>
#include <SpotLight.h>
#include <ObjectManager.h>
#include <ShaderProgram.h>
#include <Textured3DObject.h>
#include <TextureManager.h>
#include <ShaderProgramManager.h>
#include <Mesh.h>

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
	m_pAssimpObjectManager(nullptr),
	m_iShadowMapSize(1024),
	m_fGamma(0.15f),
	m_fSobelMixing(0.4f)
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
	delete m_pAssimpObjectManager;
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
	light->set_position(glm::vec3(8.7f, 4.3f, -7.f));
	light->set_color(glm::vec3(1.f, 0.65f, 0.65f));
	light->set_specular_power(10);
	light->set_intensity(1.0f);
	m_vLights.push_back(light);

	m_pProgramGUI->add_gui_element("Light 1", light->generate_slider_position_infos("Light 1 Position"));
	m_pProgramGUI->add_gui_element("Light 1", light->generate_slider_color_infos("Light 1 Color"));
	m_pProgramGUI->add_gui_element("Light 1", light->generate_slider_specular_power_infos("Light 1 Specular Power"));
	m_pProgramGUI->add_gui_element("Light 1", light->generate_slider_intensity_infos("Intensity"));
	m_pProgramGUI->add_gui_element("Light 1", light->generate_use_light_checkbox("Use Light"));

	light = new Light();
	light->set_position(glm::vec3(-8.7f, 4.3f, -7.f));
	light->set_color(glm::vec3(0.65f, 0.65f, 1.f));
	light->set_specular_power(10.f);
	light->set_intensity(1.0f);
	m_vLights.push_back(light);

	m_pProgramGUI->add_gui_element("Light 2", light->generate_slider_position_infos("Light 2 Position"));
	m_pProgramGUI->add_gui_element("Light 2", light->generate_slider_color_infos("Light 2 Color"));
	m_pProgramGUI->add_gui_element("Light 2", light->generate_slider_specular_power_infos("Light 2 Specular Power"));
	m_pProgramGUI->add_gui_element("Light 2", light->generate_slider_intensity_infos("Intensity"));
	m_pProgramGUI->add_gui_element("Light 2", light->generate_use_light_checkbox("Use Light"));

	auto dirlight = new DirectionalLight();
	dirlight->set_position(glm::vec3(1.0f, 5.0f, 1.0f));
	dirlight->set_direction(glm::vec3(0.50f, 1.0f, 1.0f));
	dirlight->set_color(glm::vec3(0.0f, 0.0f, 0.0f));
	dirlight->set_specular_power(4.0f);
	m_vLights.push_back(dirlight);

	m_pProgramGUI->add_gui_element("Directional Light", dirlight->generate_slider_position_infos("Position"));
	m_pProgramGUI->add_gui_element("Directional Light", dirlight->generate_slider_direction_infos("Direction"));
	m_pProgramGUI->add_gui_element("Directional Light", dirlight->generate_slider_specular_power_infos("Specular Power"));
	m_pProgramGUI->add_gui_element("Directional Light", dirlight->generate_slider_color_infos("Color"));
	m_pProgramGUI->add_gui_element("Directional Light", dirlight->generate_use_light_checkbox("Use Light"));


	auto spotLight = new SpotLight();
	spotLight->set_position(glm::vec3(7.f, 2.5f, -40.0f));
	spotLight->set_direction(glm::vec3(0.0f, 0.f, 0.3f));
	spotLight->set_color(glm::vec3(1.0f, 1.0f, 1.0f));
	spotLight->set_specular_power(20.0f);
	spotLight->set_angle(2.15f);
	spotLight->set_falloff_angle(3.14f);
	spotLight->set_intensity(0.8f);
	m_vLights.push_back(spotLight);

	m_pProgramGUI->add_gui_element("Spot Light", spotLight->generate_slider_position_infos("Position"));
	m_pProgramGUI->add_gui_element("Spot Light", spotLight->generate_slider_direction_infos("Direction"));
	m_pProgramGUI->add_gui_element("Spot Light", spotLight->generate_slider_color_infos("Color"));
	m_pProgramGUI->add_gui_element("Spot Light", spotLight->generate_slider_angle_infos("Angle"));
	m_pProgramGUI->add_gui_element("Spot Light", spotLight->generate_slider_fall_off_infos("Fall Off"));
	m_pProgramGUI->add_gui_element("Spot Light", spotLight->generate_slider_intensity_infos("Intensity"));
	m_pProgramGUI->add_gui_element("Spot Light", spotLight->generate_slider_specular_power_infos("Specular Power"));
	m_pProgramGUI->add_gui_element("Spot Light", spotLight->generate_use_light_checkbox("Use Light"));
}

void SceneManager::setup_shader_programs()
{
	m_pShaderProgramManager->create_main_shader_program();
	m_pShaderProgramManager->create_blit_shader_program();
	m_pShaderProgramManager->create_lighting_shader_program();
	m_pShaderProgramManager->create_pass_through_shader_program();
	m_pShaderProgramManager->create_gamma_program();
	m_pShaderProgramManager->create_sobel_program();
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
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", aiTextureType_DIFFUSE), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", aiTextureType_NORMALS), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", aiTextureType_DISPLACEMENT), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Error on building framebuffer\n");
		exit(EXIT_FAILURE);
	}

	// Back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




	//Shadow Map frame buffer
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




	// Create Fx Framebuffer Object

	GLuint fxDrawBuffers[1];
	glGenFramebuffers(1, &m_gluiFxFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gluiFxFbo);

	fxDrawBuffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, fxDrawBuffers);
	// Create Fx textures
	const int FX_TEXTURE_COUNT = 4;
	m_agluiFxTextures = new GLuint[FX_TEXTURE_COUNT];
	glGenTextures(FX_TEXTURE_COUNT, m_agluiFxTextures);
	for (int i = 0; i < FX_TEXTURE_COUNT; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, m_agluiFxTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_pProgramGUI->get_width(), m_pProgramGUI->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	// Attach first fx texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_agluiFxTextures[0], 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "Error on building framebuffern");
		exit(EXIT_FAILURE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	auto infos = new GUIInfos("Gamma value", 0.01f, 6.0f, 0.010f);
	infos->var.push_back(std::make_pair("Gamma value", &m_fGamma));

	m_pProgramGUI->add_gui_element("Gamma", infos);


	infos = new GUIInfos("Mixing Sobel value", 0.00f, 5.0f, 0.010f);
	infos->var.push_back(std::make_pair("Mixing Sobel value", &m_fSobelMixing));

	m_pProgramGUI->add_gui_element("Gamma", infos);
}

void SceneManager::setup_objects()
{
	m_pObjectManager = new ObjectManager(3);

	Textured3DObject*  cube = new Textured3DObject();
	cube->load_object(".\\Objects\\cube.aogl", true);
	int maxInstance = 5041;
	m_pObjectManager->bind_object(cube, maxInstance, 0);
	cube->set_textures(m_pTextureManager->get_textures("brick"),0);
	m_pProgramGUI->add_gui_element("Cube", m_pObjectManager->generate_slider_nb_instances_infos(0, (float)maxInstance));
	m_pProgramGUI->add_gui_element("Cube", m_pObjectManager->generate_slider_cube_size(0));
	m_pProgramGUI->add_gui_element("Cube", m_pObjectManager->generate_slider_cube_radius_spacing(0));
	m_pProgramGUI->add_gui_element("Cube", m_pObjectManager->generate_slider_cube_range(0));
	m_pProgramGUI->add_gui_element("Cube", m_pObjectManager->generate_slider_cube_speed(0));
	m_pProgramGUI->add_gui_element("Cube", m_pObjectManager->generate_check_cube_rotation(0));

	Textured3DObject* plane = new Textured3DObject();;
	plane->load_object(".\\Objects\\plane.aogl", true);
	plane->set_textures(m_pTextureManager->get_textures("stone"),0);
	m_pObjectManager->bind_object(plane, 1, 1);

	Textured3DObject*  cubeLight = new Textured3DObject();
	cubeLight->load_object(".\\Objects\\cube.aogl", true);
	cubeLight->set_name("CubeLight1");
	cubeLight->set_textures(m_pTextureManager->get_textures("stone"),0);
	m_pObjectManager->bind_object(cubeLight, 1, 0);

	cubeLight = new Textured3DObject();
	cubeLight->load_object(".\\Objects\\cube.aogl", true);
	cubeLight->set_name("CubeLight2");
	cubeLight->set_textures(m_pTextureManager->get_textures("stone"),0);
	m_pObjectManager->bind_object(cubeLight, 1, 0);


	Textured3DObject*  cubeSpotLight = new Textured3DObject();
	cubeSpotLight->load_object(".\\Objects\\cube.aogl", true);
	cubeSpotLight->set_name("CubeSpotLight");
	cubeSpotLight->set_textures(m_pTextureManager->get_textures("stone"),0);
	m_pObjectManager->bind_object(cubeSpotLight, 1, 0);


	m_pDeferredObjectManager = new ObjectManager(1);

	Textured3DObject* blit_plane = new Textured3DObject();;
	blit_plane->load_object(".\\Objects\\blit_plane.aogl", true);
	m_pDeferredObjectManager->bind_object(blit_plane, 1, 0);

	m_pAssimpObjectManager = new ObjectManager(1);

	Textured3DObject* birdy = new Textured3DObject();;
	birdy->load_object(".\\Objects\\Birds\\BirdAlone.obj", false, m_pTextureManager);
	m_pAssimpObjectManager->bind_object(birdy, 1, 0);
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
		m_iLockPositionX = (int)x;
		m_iLockPositionY = (int)y;
	}
	if (altPressed == GLFW_PRESS)
	{
		double mousex;
		double mousey;
		glfwGetCursorPos(m_pProgramGUI->get_window(), &mousex, &mousey);
		int diffLockPositionX = (int)mousex - m_iLockPositionX;
		int diffLockPositionY = (int)mousey - m_iLockPositionY;
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
		m_iLockPositionX = (int)mousex;
		m_iLockPositionY = (int)mousey;
	}
}

void SceneManager::display_scene(bool activate_deferred, bool activate_gamma, bool activate_shadow_map)
{
	//Default states
	glEnable(GL_DEPTH_TEST);

	if (activate_shadow_map)
	{
		shadow_map_management();
	}

	if (activate_deferred)
	{
		// Bind gbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_gluiBufferFbo);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Clear the front buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get camera matrices
	glm::mat4 projection = glm::perspective(45.0f, (float)m_pProgramGUI->get_width() / (float)m_pProgramGUI->get_height(), 0.1f, 100.f);
	glm::mat4 worldToView = glm::lookAt(m_pCamera->GetEye(), m_pCamera->GetO(), m_pCamera->GetUp());
	glm::mat4 objectToWorld;
	glm::mat4 mv = worldToView * objectToWorld;
	glm::mat4 mvp = projection * mv;
	glm::vec4 light = worldToView * glm::vec4(m_vLights.at(0)->get_position(), 1.0f);


	// Select shader
	auto shader = m_pShaderProgramManager->get_shader(MAIN);
	if (shader != nullptr)
	{
		glUseProgram(shader->get_program());

		draw_scene(shader, mvp, mv);

		if (activate_gamma)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_gluiFxFbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_agluiFxTextures[0], 0);
		}

		else
		{
			// Fallback to default framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		if (activate_deferred)
		{
			// Revert to window size viewport
			glViewport(0, 0, m_pProgramGUI->get_width(), m_pProgramGUI->get_height());
			illuminate_scene(mvp, mv, activate_shadow_map);
		}
		if (activate_gamma)
		{
			sobel_management(mvp, mv);
			gamma_management(mvp, mv);
		}

		if (activate_deferred)
		{
			debug_frame_buffer(mvp, mv, false, false);
		}
	}
}

void SceneManager::draw_scene(ShaderProgram * shader, glm::mat4 mvp, glm::mat4 mv)
{
	// Upload uniforms
	shader->set_var_value("CamPos", glm::value_ptr(m_pCamera->GetEye()));
	shader->set_var_value("MVP", glm::value_ptr(mvp));
	shader->set_var_value("MV", glm::value_ptr(mv));

	shader->set_var_value("Light1Pos", glm::value_ptr(m_vLights.at(0)->get_position()));
	shader->set_var_value("Light1Color", glm::value_ptr(m_vLights.at(0)->get_color()));
	shader->set_var_value("Light1SpecularPower", m_vLights.at(0)->get_specular_power());
	shader->set_var_value("Light1Use", (int)m_vLights.at(0)->get_is_in_use());

	shader->set_var_value("Light2Pos", glm::value_ptr(m_vLights.at(1)->get_position()));
	shader->set_var_value("Light2Color", glm::value_ptr(m_vLights.at(1)->get_color()));
	shader->set_var_value("Light2SpecularPower", m_vLights.at(1)->get_specular_power());
	shader->set_var_value("Light2Use", (int)m_vLights.at(1)->get_is_in_use());

	auto baseLight = m_vLights.at(2);
	auto light = (DirectionalLight*)baseLight;
	shader->set_var_value("Light3Position", glm::value_ptr(light->get_position()));
	shader->set_var_value("Light3Direction", glm::value_ptr(light->get_direction()));
	shader->set_var_value("Light3Color", glm::value_ptr(light->get_color()));
	shader->set_var_value("Light3SpecularPower", light->get_specular_power());
	shader->set_var_value("Light3Use", (int)light->get_is_in_use());

	baseLight = m_vLights.at(3);
	auto spotlight = (SpotLight*)baseLight;
	shader->set_var_value("SpotLightPosition", glm::value_ptr(spotlight->get_position()));
	shader->set_var_value("SpotLightDirection", glm::value_ptr(spotlight->get_direction()));
	shader->set_var_value("SpotLightColor", glm::value_ptr(spotlight->get_color()));
	shader->set_var_value("SpotLightIntensity", spotlight->get_intensity());
	shader->set_var_value("SpotLightSpecularPower", spotlight->get_specular_power());
	shader->set_var_value("SpotLightAngle", spotlight->get_angle());
	shader->set_var_value("SpotLightFallOffAngle", spotlight->get_falloff_angle());
	shader->set_var_value("SpotLightUse", (int)spotlight->get_is_in_use());

	spotlight->check_angles();

	for each (auto object in m_pAssimpObjectManager->get_objects())
	{
		if (object.first != nullptr)
		{
			for each (auto mesh in object.first->get_meshes())
			{
				glBindVertexArray(mesh->get_vao());

				m_pTextureManager->apply_material(mesh->get_material());

				glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
			}
		}
	}

	for each (auto object in m_pObjectManager->get_objects())
	{
		if (object.first != nullptr)
		{
			for each (auto mesh in object.first->get_meshes())
			{
				glBindVertexArray(mesh->get_vao());

				// Select textures
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->get_texture(aiTextureType_DIFFUSE));
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, mesh->get_texture(aiTextureType_SPECULAR));

				if (object.first->get_name() == "cube")
				{
					shader->set_var_value("isCube", (int)true);
					if (object.first->check_start_rotation())
					{
						object.first->set_rotating_start(m_pProgramGUI->get_time());
					}
					shader->set_var_value("Time", (float)(m_pProgramGUI->get_time() - object.first->get_rotating_start()));

					shader->set_var_value("Rotate", (int)*object.first->is_rotating());
					shader->set_var_value("InstanceNumber", *object.second);
					shader->set_var_value("SizeFactor", *object.first->get_size());
					shader->set_var_value("RadiusSpacing", *object.first->get_radius_spacing());
					shader->set_var_value("SpeedFactor", *object.first->get_speed());
					shader->set_var_value("RangeFactor", *object.first->get_range());
					glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)(mesh->get_triangles_count() * 3), GL_UNSIGNED_INT, (void*)0, (GLsizei)(*object.second));
					shader->set_var_value("isCube", (int)false);
				}
				else if (object.first->get_name() == "CubeLight1" && m_vLights.at(0)->get_is_in_use())
				{
					shader->set_var_value("isLight1Marker", (int)true);
					glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
					shader->set_var_value("isLight1Marker", (int)false);
				}

				else if (object.first->get_name() == "CubeLight2" && m_vLights.at(1)->get_is_in_use())
				{
					shader->set_var_value("isLight2Marker", (int)true);
					glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
					shader->set_var_value("isLight2Marker", (int)false);
				}

				else if (object.first->get_name() == "CubeSpotLight" && spotlight->get_is_in_use())
				{
					shader->set_var_value("isSpotLightMarker", (int)true);
					glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
					shader->set_var_value("isSpotLightMarker", (int)false);
				}
				else
				{
					glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
				}
			}
		}
	}
}

bool SceneManager::illuminate_scene(glm::mat4 mvp, glm::mat4 mv, bool activate_shadow_map)
{
	glClear(GL_COLOR_BUFFER_BIT);
	// Enable blending
	glEnable(GL_BLEND);
	// Setup additive blending
	glBlendFunc(GL_ONE, GL_ONE);

	//Render Lighting
	// Use deferred point light shader
	auto shader = m_pShaderProgramManager->get_shader(LIGHT);
	if (shader != nullptr)
	{
		glUseProgram(shader->get_program());
		// Compute the inverse worldToScreen matrix
		glm::mat4 screenToWorld = glm::transpose(glm::inverse(mvp));

		// Bind quad vao
		auto mesh = m_pDeferredObjectManager->get_object(0).first->get_meshes()[0];
		glBindVertexArray(mesh->get_vao());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", aiTextureType_DIFFUSE));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", aiTextureType_NORMALS));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", aiTextureType_DISPLACEMENT));

		auto baseLight = m_vLights.at(3);
		auto spotlight = (SpotLight*)baseLight;
		shader->set_var_value("SpotLightPosition", glm::value_ptr(spotlight->get_position()));
		shader->set_var_value("SpotLightDirection", glm::value_ptr(spotlight->get_direction()));
		shader->set_var_value("SpotLightColor", glm::value_ptr(spotlight->get_color()));
		shader->set_var_value("SpotLightIntensity", spotlight->get_intensity());
		shader->set_var_value("SpotLightSpecularPower", spotlight->get_specular_power());
		shader->set_var_value("SpotLightAngle", spotlight->get_angle());
		shader->set_var_value("SpotLightFallOffAngle", spotlight->get_falloff_angle());


		baseLight = m_vLights.at(2);
		auto light = (DirectionalLight*)baseLight;
		shader->set_var_value("Light3Position", glm::value_ptr(light->get_position()));
		shader->set_var_value("Light3Direction", glm::value_ptr(light->get_direction()));
		shader->set_var_value("Light3Color", glm::value_ptr(light->get_color()));
		shader->set_var_value("Light3SpecularPower", light->get_specular_power());


		if (activate_shadow_map)
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, m_gluiShadowTexture);

			glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
		}
		else
		{
			shader->set_var_value("ScreenToWorld", glm::value_ptr(screenToWorld));
			for (int i = 0; i < 2; i++)
			{
				if (m_vLights.at(i)->get_is_in_use())
				{
					shader->set_var_value("ScreenToWorld", glm::value_ptr(screenToWorld));
					shader->set_var_value("LightPos", glm::value_ptr(m_vLights.at(i)->get_position()));
					shader->set_var_value("LightColor", glm::value_ptr(m_vLights.at(i)->get_color()));
					shader->set_var_value("LightIntensity", m_vLights.at(i)->get_intensity());
					shader->set_var_value("SpotLightUse", (int)false);
					// Render quad
					glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
				}
			}
			shader->set_var_value("SpotLightUse", (int)spotlight->get_is_in_use());
			glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
		}
		// Disable blending
		glDisable(GL_BLEND);
		glBindVertexArray(0);
		return true;
	}
	return false;
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
		// From world to light
		auto baseLight = m_vLights.at(3);
		auto light = (SpotLight*)baseLight;
		glm::mat4 worldToLight = glm::lookAt(light->get_position(), light->get_position() + light->get_direction(), m_pCamera->GetUp()/* glm::vec3(0.f, 0.f, -1.f)*/);
		glm::mat4 projection = glm::perspective(glm::radians(light->get_angle()), 1.f, 0.1f, 100.f);

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

		draw_scene(shader, objectToLightScreen, objectToLight);

		// Unbind the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_pProgramGUI->get_width(), m_pProgramGUI->get_height());
	}

}

void SceneManager::sobel_management(glm::mat4 mvp, glm::mat4 mv)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_agluiFxTextures[1], 0);
	// Clear default framebuffer color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	auto shader = m_pShaderProgramManager->get_shader(SOBEL);
	if (shader != nullptr)
	{
		glUseProgram(shader->get_program());
		shader->set_var_value("Mix", m_fSobelMixing);
		auto mesh = m_pDeferredObjectManager->get_object(0).first->get_meshes()[0];
		glBindVertexArray(mesh->get_vao());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_agluiFxTextures[0]);
		glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}
}

void SceneManager::gamma_management(glm::mat4 mvp, glm::mat4 mv)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_agluiFxTextures[2], 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Clear default framebuffer color buffer
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	auto shaderGamma = m_pShaderProgramManager->get_shader(GAMMA);
	if (shaderGamma != nullptr)
	{
		glUseProgram(shaderGamma->get_program());
		shaderGamma->set_var_value("Gamma", m_fGamma);
		auto mesh = m_pDeferredObjectManager->get_object(0).first->get_meshes()[0];
		glBindVertexArray(mesh->get_vao());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_agluiFxTextures[1]);
		glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}
}

void SceneManager::debug_frame_buffer(glm::mat4 mvp, glm::mat4 mv, bool activate_shadow_map, bool activate_sobel_map)
{
	// Render Debug information
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, m_pProgramGUI->get_width(), m_pProgramGUI->get_height());

	auto mesh = m_pDeferredObjectManager->get_object(0).first->get_meshes()[0];
	glBindVertexArray(mesh->get_vao());
	glActiveTexture(GL_TEXTURE0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	auto blitShader = m_pShaderProgramManager->get_shader(BLIT);
	if (blitShader != nullptr)
	{
		auto object = m_pDeferredObjectManager->get_object(0);
		glBindVertexArray(mesh->get_vao());
		// Use the blit program
		glUseProgram(blitShader->get_program());
		
		int nbviewport = 4;
		if (activate_shadow_map)
		{
			nbviewport++;
		}
		if (activate_sobel_map)
		{
			nbviewport++;
		}
		
		int smallWidth = m_pProgramGUI->get_width() / nbviewport;
		int smallHeight = m_pProgramGUI->get_height() / nbviewport;
		int currVP = 0;
	
		glActiveTexture(GL_TEXTURE0);
		glViewport(0, 0, smallWidth, smallHeight);
		// Bind gbuffer color texture
		glBindTexture(GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", aiTextureType_DIFFUSE));
		// Draw quad
		glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
		currVP++;

		glViewport(currVP*m_pProgramGUI->get_width() / nbviewport, 0, smallWidth, smallHeight);
		// Bind gbuffer color texture
		glBindTexture(GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", aiTextureType_NORMALS));
		// Draw quad
		glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
		currVP++;

		glViewport(currVP * m_pProgramGUI->get_width() / nbviewport, 0, smallWidth, smallHeight);
		// Bind gbuffer color texture
		glBindTexture(GL_TEXTURE_2D, m_pTextureManager->get_texture("deferred", aiTextureType_DISPLACEMENT));
		// Draw quad
		glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
		currVP++;


		glViewport(currVP * m_pProgramGUI->get_width() / nbviewport, 0, smallWidth, smallHeight);
		// Bind gbuffer color texture
		glBindTexture(GL_TEXTURE_2D, m_agluiFxTextures[0]);
		// Draw quad
		glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
		currVP++;

		if (activate_shadow_map)
		{
			glViewport(currVP * m_pProgramGUI->get_width() / nbviewport, 0, smallWidth, smallHeight);
			glBindTexture(GL_TEXTURE_2D, m_gluiShadowTexture);
			// Draw quad
			glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
			currVP++;
		}
		if (activate_sobel_map)
		{
			glViewport(currVP * m_pProgramGUI->get_width() / nbviewport, 0, smallWidth, smallHeight);
			glBindTexture(GL_TEXTURE_2D, m_agluiFxTextures[1]);
			// Draw quad
			glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
			currVP++;
		}
	}
}
void SceneManager::setupdemo()
{
	auto spotLight = (SpotLight*)m_vLights.at(3);
	spotLight->set_color(glm::vec3(0.5, 0.4, 0.4));
	spotLight->set_position(glm::vec3(0., 3.0, -10.));
	spotLight->set_direction(glm::vec3(0.0, 0.20, 1.0));
	spotLight->set_intensity(1.7f);
	spotLight->set_is_in_use(true);
	m_fGamma = 0.2f;
	m_fSobelMixing = 0.7f;

	auto light1 = m_vLights.at(0);
	light1->set_is_in_use(true);
	light1->set_position(glm::vec3(2.0f, 3.5f, 5.5f));
	light1->set_intensity(1.2f);

	auto light2 = m_vLights.at(1);
	light2->set_is_in_use(true);
	light2->set_position(glm::vec3(-2.0f, 3.5f, 5.5f));
	m_fGamma = 0.2f;
	spotLight->set_intensity(1.35f);
	light1->set_intensity(1.15f);
	light2->set_intensity(1.15f);
	light2->set_color(spotLight->get_color() + glm::vec3(0.1f, 0.0f, 0.0f));/**/
}

void SceneManager::demo(SceneManager * scn, double start)
{
	scn->m_bDemoRunning = true;
	scn->m_dStartTime = start;

	/*auto baseLight = scn->m_vLights[2];
	auto dirlight = (DirectionalLight*)baseLight;
	while (scn->m_pProgramGUI->get_time() < scn->m_dStartTime + 5)
	{
	Sleep(50);
	}*/
	Textured3DObject * cube = nullptr;

	float * nbinstance = nullptr;
	for each (auto object in scn->m_pObjectManager->get_objects())
	{
		if (object.first->get_name() == "cube")
		{
			cube = object.first;
			nbinstance = object.second;
			*nbinstance = 1;
		}
	}

	Sleep(10000);
	auto spotLight = (SpotLight*)scn->m_vLights.at(3);
	spotLight->set_color(glm::vec3(0.5, 0.4, 0.4));
	spotLight->set_position(glm::vec3(0., 3.0, -10.));
	spotLight->set_direction(glm::vec3(0.0, 0.20, 1.0));
	spotLight->set_intensity(1.7f);
	spotLight->set_is_in_use(true);
	scn->m_fGamma = 0.3f;
	scn->m_fSobelMixing = 0.0f;
	Sleep(2000);
	auto light1 = scn->m_vLights.at(0);
	light1->set_color(glm::vec3(0.35f, 0.34f, 0.1f));

	light1->set_is_in_use(true);
	light1->set_position(glm::vec3(2.0f, 3.5f, 5.5f));
	light1->set_intensity(1.2f);
	Sleep(2000);
	auto light2 = scn->m_vLights.at(1);
	light2->set_color(glm::vec3(0.35f, 0.34f, 0.1f));
	light2->set_is_in_use(true);
	light2->set_position(glm::vec3(-2.0f, 3.5f, 5.5f));
	scn->m_fGamma = 0.2f;
	spotLight->set_intensity(1.35f);
	light1->set_intensity(1.15f);
	light2->set_intensity(1.15f);
	light2->set_color(spotLight->get_color() + glm::vec3(0.1f, 0.0f, 0.0f));
	Sleep(2000);
	bool done = false;
	while (done == false)
	{
		done = true;
		if (spotLight->get_position().z > -50.0)
		{
			spotLight->set_position(spotLight->get_position() + glm::vec3(.0f, 0.0f, -0.2f));
			done &= false;
		}
		if (spotLight->get_position().y < 50.0)
		{
			spotLight->set_position(spotLight->get_position() + glm::vec3(.0f, 0.2f, 0.0f));
			done &= false;
		}
		if (spotLight->get_direction().y > -0.4)
		{
			spotLight->set_direction(spotLight->get_direction() + glm::vec3(.0f, -0.005f, 0.0f));
			done &= false;
		}
		if (spotLight->get_direction().z < 0.2)
		{
			spotLight->set_direction(spotLight->get_direction() + glm::vec3(.0f, 0.f, 0.005f));
			done &= false;
		}
		if (light1->get_intensity() < 1.8)
		{
			light1->set_intensity(light1->get_intensity() + 0.0071f);
			light2->set_intensity(light2->get_intensity() + 0.0071f);
		}

		if (spotLight->get_intensity() > 0.8)
		{
			spotLight->set_intensity(spotLight->get_intensity() - 0.01f);
			done &= false;
		}
		Sleep(40);
	}

	done = false;
	while (done == false)
	{
		done = true;
		if (light1->get_position().x < 5.5)
		{
			light1->set_position(light1->get_position() + glm::vec3(.01f, 0.0f, 0.f));
			done &= false;
		}
		if (light1->get_position().y < 8)
		{
			light1->set_position(light1->get_position() + glm::vec3(.0f, 0.01f, 0.f));
			done &= false;
		}
		if (light1->get_position().z > 2.5)
		{
			light1->set_position(light1->get_position() + glm::vec3(.0f, 0.0f, -0.01f));
			done &= false;
		}

		if (light2->get_position().x > -5.5)
		{
			light2->set_position(light2->get_position() + glm::vec3(-.01f, 0.0f, 0.f));
			done &= false;
		}
		if (light2->get_position().y < 8)
		{
			light2->set_position(light2->get_position() + glm::vec3(.0f, 0.01f, 0.f));
			done &= false;
		}
		if (light2->get_position().z > 2.5)
		{
			light2->set_position(light2->get_position() + glm::vec3(.0f, 0.0f, -0.01f));
			done &= false;
		}
		/*	if (light1->get_intensity() < 1.9)
			{
			light1->set_intensity(light1->get_intensity() + 0.01);
			done &= false;
			}
			if (light2->get_intensity() < 1.9)
			{
			light2->set_intensity(light2->get_intensity() + 0.01);
			done &= false;
			}*/
		(*nbinstance) += 1;
		Sleep(20);
	}

	done = false;
	while (done == false)
	{
		done = true;
		if (scn->m_fGamma < 0.23)
		{
			scn->m_fGamma += 0.001f;
			done &= false;
		}
		Sleep(50);
	}


	//Sleep(2000);
	while (*nbinstance < 5000)
	{
		(*nbinstance) += 100;
		float size = *(cube->get_size());
		if (size > 0.1)
		{
			cube->set_size(size - 0.005f);
		}
		Sleep(20);
	}
	*nbinstance = 5041;
	bool firstpart = false;
	bool secondpart = false;
	bool thirdpart = false;
	bool fourthpart = false;
	while (scn->m_bDemoRunning)
	{
		if (firstpart == false)
		{
			if (*(cube->get_range()) < 0.9)
			{
				cube->set_range((*(cube->get_range()) + 0.001f));
			}
			if (*(cube->get_radius_spacing()) > 0.8)
			{
				cube->set_radius_spacing((*(cube->get_radius_spacing()) - 0.005f));
			}
			if ((*(cube->get_radius_spacing()) < 0.81 && *(cube->get_range()) > 0.5) && *(cube->get_speed()) < 1)
			{
				cube->set_speed((*(cube->get_speed()) + 0.0005f));
			}
			if (*(cube->get_speed()) > 0.3)
			{
				firstpart = true;
			}
			if (*(cube->get_size()) > 0.1)
			{
				cube->set_size(*(cube->get_size()) - 0.001f);
			}
			//
			//light1->set_color(glm::vec3(sin(time / 1000.0f), tan(time / 1000.0f), cos(time / 1000.0f)));
			//light2->set_color(glm::vec3(cos(time / 1000.0f), sin(time / 1000.0f), cos(time / 1000.0f)));
			//spotLight->set_color(glm::vec3(tan(time / 1000.0f), tan(time / 1000.0f), sin(time / 1000.0f)));
			Sleep(30);
		}

		if (firstpart && secondpart == false)
		{
			Sleep(5000);
			secondpart = true;
		}

		if (secondpart && thirdpart == false)
		{
			done = true;
			if (*(cube->get_range()) > 0)
			{
				cube->set_range((*(cube->get_range()) - 0.001f));
				done &= false;
			}
			if (*(cube->get_speed()) > 0)
			{
				cube->set_speed((*(cube->get_speed()) - 0.0005f));
				done &= false;
			}
			if (done)
			{
				thirdpart = true;
			}
			Sleep(20);
		}
		if (thirdpart && fourthpart == false)
		{
			done = true;
			if (*(cube->is_rotating()) != true)
			{
				cube->set_rotating(true);
				done &= false;
			}

			if (*(cube->get_speed()) < 40.0)
			{
				cube->set_speed((*(cube->get_speed()) + 0.05f));
				done &= false;
			}
			if (done)
			{
				fourthpart = true;
			}
			Sleep(60);
		}
		if (fourthpart)
		{
			if (*(cube->get_speed()) > 0)
			{
				cube->set_speed((*(cube->get_speed()) - 0.05f));
			}
			Sleep(60);
		}
	}
}

void SceneManager::updateBird(const glm::vec3& birdPosition, float birdAngle)
{
  glm::mat4 model(1.f);
  model = glm::rotate(model, birdAngle, glm::vec3(0, 1, 0));

  //Set uniform in shader TODO
}

void SceneManager::updatePredators(const std::vector<glm::vec3>& predatorsPositions, const std::vector<glm::vec3>& predatorsDirections)
{

}
