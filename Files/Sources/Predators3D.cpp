#include <Predators3D.h>
#include <ShaderProgram.h>
#include <Mesh.h>
#include <ObjectManager.h>
#include <ShaderProgramManager.h>

using namespace SweepingBirds;

Predators3D::Predators3D()
{
	m_eShaderType = PREDATOR;
}

Predators3D::Predators3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance)
	:Textured3DObject(texMgr)
{
	m_eShaderType = PREDATOR;
	m_pObjectManager = manager;
	load_object(".\\Objects\\Bats\\Bats.obj", false, m_pTextureManager);
	m_pObjectManager->bind_object(this, nbInstance);

	set_position(glm::vec3(-0.0f, 0.0f, 0.0f));
	//set_rotation_angle(-M_PI / 2.0f);
	set_size(1.0f);
	//set_object_type(BAT);
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider_nb_instances_infos("Predators", nbInstance));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("SizeFactor", 0.01f, 20.0f, 0.1f, get_size()));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("PosX", -500.0f, 500.0f, 1.f, get_x_pos()));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("PosY", -500.0f, 500.0f, 1.f, get_y_pos()));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("PosZ", -500.0f, 500.0f, 1.f, get_z_pos()));
}


Predators3D::~Predators3D()
{
}

void Predators3D::draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance)
{
	for each (auto mesh in m_vMeshes)
	{
		auto shader = setup_drawing_space(shaderMgr, mesh, proj, time);
		if (shader != nullptr)
		{
			//m_bPredatorsData.activate(PREDATORS_BINDING);

			shader->set_var_value("InstanceNumber", (int)sqrt(nbInstance));
			glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)(mesh->get_triangles_count() * 3), GL_UNSIGNED_INT, (void*)0, (GLsizei)nbInstance);

			//m_bPredatorsData.deactivate();
			clean_bindings();
		}
	}
}