#include <Ground3D.h>
#include <ShaderProgram.h>
#include <Mesh.h>
#include <ObjectManager.h>
#include <SceneManager.h>
#include <ShaderProgramManager.h>
#include <Camera.h>
#include <DirectionalLight.h>
#include <GPUBuffer.h>
#include <DirectionalLight.h>
#include <Light.h>

using namespace SweepingBirds;
const GLuint Ground3D::HEIGHTMAP_BINDING = GL_TEXTURE6;
const GLuint Ground3D::NORMALMAP_BINDING = GL_TEXTURE7;

Ground3D::Ground3D()
{
	
}

Ground3D::Ground3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance)
	:Textured3DObject(texMgr)
{
	m_bufHeightMapGPUBuffer = new GPUBuffer(GL_R32F);
	m_bufHeightMapGPUBuffer->setData(sizeof(float) * 961 * nbInstance, nullptr);
	m_bufNormalMapGPUBuffer = new GPUBuffer(GL_RGB32F);
	m_bufNormalMapGPUBuffer->setData(sizeof(float) * 961 * nbInstance * 3, nullptr);
	m_eShaderType = GROUND;
	m_pObjectManager = manager;
	load_object(".\\Objects\\Ground\\Ground.obj", false, m_pTextureManager);
	m_pObjectManager->bind_object(this, nbInstance);
	m_fHeight = 1300.0f;
	m_fSize = 1.f;
	m_fMountainFrequency = 270.f;
	m_iPatchSize = 1800;
	set_position(glm::vec3(0.0, -500.0, 0.0));
	m_ftest1 = 3;
	m_ftest2 = 0.0005;
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider_nb_instances_infos(this, nbInstance));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("Mountain Height", 0.0f, 5000.0f, 10.0f, &m_fHeight));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("test1", 0.0f, 6.f, 0.01f, &m_ftest1));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("test2", 0.0f, 0.01f, 0.0001f, &m_ftest2));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("Mountain Frequency", 0.0f, 5000.0f, 10.0f, &m_fMountainFrequency));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("Patch Control", 0.0f, 5000.0f, 10.0f, &m_iPatchSize));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("PosX", -500.0f, 500.0f, 1.f, get_x_pos()));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("PosY", -2500.0f, -500.0f, 1.f, get_y_pos()));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("PosZ", -500.0f, 500.0f, 1.f, get_z_pos()));
}


Ground3D::~Ground3D()
{
}

void Ground3D::update(const float * heightMap, int heightMapSize, const float * normalMap, int normalMapSize/*const std::map<int, std::vector<float>>& normalMap*/)
{
	/*std::vector<float> heightVal;
	for (auto keyValue : heightMap)
	{
		heightVal.insert(heightVal.end(), keyValue.second.begin(), keyValue.second.end());
	}*/
	m_bufHeightMapGPUBuffer->updateData(heightMap, 0, heightMapSize*sizeof(float));
	/*std::vector<float> normalVal;
	for (auto keyValue : normalMap)
	{
		normalVal.insert(normalVal.end(), keyValue.second.begin(), keyValue.second.end());
	}
	m_bufNormalMapGPUBuffer->updateData(normalVal.data(), 0, normalVal.size()*sizeof(float));*/
	m_bufNormalMapGPUBuffer->updateData(normalMap, 0, normalMapSize *sizeof(float));
}


void Ground3D::draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance)
{
	for each (auto mesh in m_vMeshes)
	{
		auto shader = setup_drawing_space(shaderMgr, mesh, proj, time);
		if (shader != nullptr)
		{
			glm::mat4 Model;
			auto cam = m_pObjectManager->get_Camera();
			auto ModelTranslated = glm::translate(Model, cam->GetEye());
			shader->set_var_value("GroundTranslation", glm::value_ptr(ModelTranslated));
			shader->set_var_value("InstanceNumber", nbInstance);
			shader->set_var_value("SquareSideLength", (int)sqrt(nbInstance));
			shader->set_var_value("MaxMountainHeight", m_fHeight);
			shader->set_var_value("MountainFrequence", m_fMountainFrequency);
			shader->set_var_value("PatchControl", m_iPatchSize);
			shader->set_var_value("m_ftest1", m_ftest1);
			shader->set_var_value("m_ftest2", m_ftest2);

			auto lights = m_pObjectManager->get_scene_manager()->get_lights();
			if (lights.size() > 0)
			{
				auto light = lights[0];
				auto dirLight = dynamic_cast<DirectionalLight*>(light);
				if (dirLight  != nullptr)
				{
					shader->set_var_value("LightPosition", glm::value_ptr(dirLight->get_position()));
					shader->set_var_value("LightDirection", glm::value_ptr(dirLight->get_direction()));
					shader->set_var_value("LightColor", glm::value_ptr(dirLight->get_color()));
					shader->set_var_value("LightSpecularPower", dirLight->get_specular_power());
					shader->set_var_value("LightUse",(int) dirLight->get_is_in_use());
				}
			}
			//Failed test to draw only ground tiles in front of the camera
			//auto dir = m_pCamera->GetO() - m_pCamera->GetEye();
			//dir.y = 0.0f;
			//shader->set_var_value("LookDirection", glm::value_ptr(glm::normalize(dir)));
			//m_bufHeightMapGPUBuffer->activate(HEIGHTMAP_BINDING);
			m_bufNormalMapGPUBuffer->activate(NORMALMAP_BINDING);
			glDrawElementsInstanced(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0, (GLsizei)(nbInstance));
			m_bufNormalMapGPUBuffer->deactivate();
			//m_bufHeightMapGPUBuffer->deactivate();
			clean_bindings();
		}
	}
}