#include <Ground3D.h>
#include <ShaderProgram.h>
#include <Mesh.h>
#include <ObjectManager.h>
#include <ShaderProgramManager.h>
#include <Camera.h>

using namespace SweepingBirds;

Ground3D::Ground3D()
{

}

Ground3D::Ground3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance)
	:Textured3DObject(texMgr)
{
	m_eShaderType = GROUND;
	m_pObjectManager = manager;
	load_object(".\\Objects\\Ground\\Ground.obj", false, m_pTextureManager);
	m_pObjectManager->bind_object(this, nbInstance);
	m_fHeight = 300.0f;
	//set_speed(0.f);
	m_fSize = 1.f;
	m_fMountainFrequency = 270.0f;
	m_iPatchSize = 1800;
	set_position(glm::vec3(0.0, -500.0, 0.0));
	//set_object_type(GROUND);
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider_nb_instances_infos("Ground", nbInstance));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("Mountain Height", 0.0f, 5000.0f, 10.0f, &m_fHeight));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("Mountain Frequency", 0.0f, 5000.0f, 10.0f, &m_fMountainFrequency));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("Patch Control", 0.0f, 5000.0f, 10.0f, &m_iPatchSize));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("PosX", -500.0f, 500.0f, 1.f, get_x_pos()));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("PosY", -2500.0f, -500.0f, 1.f, get_y_pos()));
	m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("PosZ", -500.0f, 500.0f, 1.f, get_z_pos()));
	//m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("Speed", 0.0f, 1000.0f, 1.f, get_speed()));
	//m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("SizeFactor", 0.01f, 3.0f, 0.1f, get_size()));

	//m_pObjectManager->add_gui_controller("Ground", m_pObjectManager->generate_slider("Mountain Frequency", 1.0, 5000.0f, 10.0f, get_radius_spacing()));
}


Ground3D::~Ground3D()
{
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
			//Failed test to draw only ground tiles in front of the camera
			//auto dir = m_pCamera->GetO() - m_pCamera->GetEye();
			//dir.y = 0.0f;
			//shader->set_var_value("LookDirection", glm::value_ptr(glm::normalize(dir)));
			glDrawElementsInstanced(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0, (GLsizei)(nbInstance));
			clean_bindings();
		}
	}
}