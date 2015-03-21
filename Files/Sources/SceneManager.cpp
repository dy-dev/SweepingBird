#include <windows.h>

#include <glew/glew.h>
#include <GLFW/glfw3.h>


#include <Skybox.h>

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
#include <GPUBuffer.h>

#define M_PI 3.14
using namespace SweepingBirds;

const float SceneManager::MOUSE_PAN_SPEED = 1.f;
const float SceneManager::MOUSE_ZOOM_SPEED = 0.05f;
const float SceneManager::MOUSE_TURN_SPEED = 0.005f;
const GLuint SceneManager::PREDATORS_BINDING = GL_TEXTURE5;

SceneManager::SceneManager()
	:m_iLockPositionX(0),
	m_iLockPositionY(0),
	m_bPanLock(false),
	m_bTurnLock(false),
	m_bZoomLock(false),
	m_pAssimpObjectManager(nullptr),
	m_fGamma(0.15f),
	//m_pSkyBox(nullptr),
  m_bPredatorsData(GL_RGB32F)
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
	delete m_pAssimpObjectManager;
}

void SceneManager::init()
{
	m_pCamera->Camera_defaults();
	setup_lights();
	setup_shader_programs();
	setup_frame_buffer();
	setup_objects();
}

void SceneManager::setup_lights()
{
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
}

void SceneManager::setup_shader_programs()
{
	m_pShaderProgramManager->create_main_shader_program();
	m_pShaderProgramManager->create_lighting_shader_program();
	m_pShaderProgramManager->create_gamma_program();
}

void SceneManager::setup_frame_buffer()
{
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
}

bool SceneManager::setup_objects()
{
	assert(m_pProgramGUI != nullptr && m_pTextureManager != nullptr);
	m_pAssimpObjectManager = new ObjectManager(m_pTextureManager, m_pProgramGUI);
	return m_pAssimpObjectManager->create_scene_assets();
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

void SceneManager::setup_predators(int maxPredators)
{
 // m_bPredatorsData.setData(maxPredators * sizeof(glm::vec3), static_cast<GLvoid*>(nullptr));
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
//				m_pGameCamera->OnMouseTurn(diffLockPositionX * MOUSE_PAN_SPEED, diffLockPositionY * MOUSE_PAN_SPEED);
				//m_pGameCamera->OnMouseMove(mousex, mousey);
			}
			else
			{
				if (m_bPanLock)
				{
					m_pCamera->Camera_pan(diffLockPositionX * MOUSE_PAN_SPEED, diffLockPositionY * MOUSE_PAN_SPEED);
			//		m_pGameCamera->OnMouseTurn(diffLockPositionX * MOUSE_PAN_SPEED, diffLockPositionY * MOUSE_PAN_SPEED);
					//
				}
			}
		}
		m_iLockPositionX = (int)mousex;
		m_iLockPositionY = (int)mousey;
	}

}


void SceneManager::display_scene(bool activate_gamma)
{
	//Default states
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

//	m_pGameCamera->OnRender();
	// Clear the front buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*	if (m_pSkyBox != nullptr)
	{
		m_pSkyBox->Render();
	}*/

	// Get camera matrices
	glm::mat4 projection = glm::perspective(70.0f, (float)m_pProgramGUI->get_width() / (float)m_pProgramGUI->get_height(), 0.1f, 100000.0f);
	glm::mat4 worldToView = glm::lookAt(m_pCamera->GetEye(), m_pCamera->GetO(), m_pCamera->GetUp());
	glm::vec4 light = worldToView * glm::vec4(m_vLights.at(0)->get_position(), 1.0f);

	// Select shader
	auto shader = m_pShaderProgramManager->get_shader(MAIN);
	if (shader != nullptr)
	{
		glUseProgram(shader->get_program());

		draw_scene(shader, projection, worldToView);

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

		if (activate_gamma)
		{
			//gamma_management(mvp, mv);
		}
	}
}

void SceneManager::draw_scene(ShaderProgram * shader, glm::mat4 proj, glm::mat4 wtv)
{
	// Upload uniforms
	shader->set_var_value("CamPos", glm::value_ptr(m_pCamera->GetEye()));
//	shader->set_var_value("BirdTranslation", glm::value_ptr(m_pBird->get_mock_pos()));

	auto baseLight = m_vLights.at(0);
	auto light = (DirectionalLight*)baseLight;
	shader->set_var_value("DirLightPosition", glm::value_ptr(light->get_position()));
	shader->set_var_value("DirLightDirection", glm::value_ptr(light->get_direction()));
	shader->set_var_value("DirLightColor", glm::value_ptr(light->get_color()));
	shader->set_var_value("DirLightSpecularPower", light->get_specular_power());
	shader->set_var_value("DirLightUse", (int)light->get_is_in_use());
	if (m_pAssimpObjectManager != nullptr)
	{
		for each (auto object in m_pAssimpObjectManager->get_objects())
		{
			if (object.second.first != nullptr)
			{
				draw_object(object.second, shader, proj, wtv);
			}
		}
	}
}

void SceneManager::draw_object(std::pair<Textured3DObject *, int*> object, ShaderProgram * shader, glm::mat4 proj, glm::mat4 wtv)
{
	/*auto posogl = glm::vec3(m_pGameCamera->GetPos().x, m_pGameCamera->GetPos().y, m_pGameCamera->GetPos().z);
			auto targetogl = glm::vec3(m_pGameCamera->GetTarget().x, m_pGameCamera->GetTarget().y, m_pGameCamera->GetTarget().z);
			auto upogl = glm::vec3(m_pGameCamera->GetUp().x, m_pGameCamera->GetUp().y, m_pGameCamera->GetUp().z);
			//glm::mat4 worldToView = glm::lokAt(posogl, targetogl, upogl);*/


	for each (auto mesh in object.first->get_meshes())
	{
		glBindVertexArray(mesh->get_vao());
		m_pTextureManager->apply_material(mesh->get_material());
		//shader->set_var_value("Translation", glm::value_ptr(object.first->get_position()));
		shader->set_var_value("Time", (float)m_pProgramGUI->get_time());
		shader->set_var_value("SizeFactor", *object.first->get_size());

		glm::mat4 Model;
		/*auto ModelRotateY = glm::rotate(Model, object.first->get_rotation_angle(), glm::vec3(0.0f, 1.0f, 0.0f));
		auto ModelTranslated = glm::translate(ModelRotateY, object.first->get_position());
		glm::mat4 ModelScaled = glm::scale(ModelTranslated, glm::vec3(*object.first->get_size()));*/

		auto ModelTranslated = glm::translate(Model, object.first->get_position());
		glm::mat4 ModelScaled = glm::scale(ModelTranslated, glm::vec3(*object.first->get_size()));
		glm::mat4 mv = wtv * ModelScaled;
		glm::mat4 mvp = proj * mv;
		shader->set_var_value("MVP", glm::value_ptr(mvp));
		shader->set_var_value("MV", glm::value_ptr(mv));
	//	shader->set_var_value("ObjectId", (int)object.first->get_object_type());

		if (object.first->get_name() == "BeeBird")
		{
			shader->set_var_value("isBird", (int)true);
			glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			shader->set_var_value("isBird", (int)false);
		}
		else if (object.first->get_name() == "Ground")
		{
			shader->set_var_value("isGround", (int)true);
			shader->set_var_value("InstanceNumber", (int)*object.second);
			shader->set_var_value("SquareSideLength", (int)sqrt(*object.second));
			/*shader->set_var_value("MaxMountainHeight", *object.first->get_height());
			shader->set_var_value("MountainFrequence", *object.first->get_radius_spacing());
			shader->set_var_value("ColorControl", *object.first->get_range());
			shader->set_var_value("SpeedFactor", *object.first->get_speed());*/
			glDrawElementsInstanced(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0, (GLsizei)(*object.second));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			shader->set_var_value("isGround", (int)false);
		}

		else if (object.first->get_name() == "Bats")
		{
      m_bPredatorsData.activate(PREDATORS_BINDING);

			shader->set_var_value("isPredator", (int)true);
			shader->set_var_value("InstanceNumber", *object.second);
			glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)(mesh->get_triangles_count() * 3), GL_UNSIGNED_INT, (void*)0, (GLsizei)(*object.second));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			shader->set_var_value("isPredator", (int)false);

      m_bPredatorsData.deactivate();
		}
		else if (object.first->get_name() == "SkyBox")
		{
			GLint OldCullFaceMode;
			glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
			GLint OldDepthFuncMode;
			glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

			glCullFace(GL_FRONT);
			glDepthFunc(GL_LEQUAL);

			shader->set_var_value("isSkyBox", (int)true);
			glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
			shader->set_var_value("isSkyBox", (int)false);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glCullFace(OldCullFaceMode);
			glDepthFunc(OldDepthFuncMode);
		}
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
		auto mesh = m_pAssimpObjectManager->get_object(0).first->get_meshes()[0];
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

	auto mesh = m_pAssimpObjectManager->get_object(0).first->get_meshes()[0];
	glBindVertexArray(mesh->get_vao());
	glActiveTexture(GL_TEXTURE0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	auto blitShader = m_pShaderProgramManager->get_shader(BLIT);
	if (blitShader != nullptr)
	{
		auto object = m_pAssimpObjectManager->get_object(0);
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
	}
}

void SceneManager::setupdemo()
{
}

void SceneManager::demo(SceneManager * scn, double start)
{
	scn->m_bDemoRunning = true;
	scn->m_dStartTime = start;
}