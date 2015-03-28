#include <Predators3D.h>
#include <ShaderProgram.h>
#include <Mesh.h>
#include <ObjectManager.h>
#include <ShaderProgramManager.h>

using namespace SweepingBirds;

const GLuint Predators3D::PREDATORS_BINDING = GL_TEXTURE5;

Predators3D::Predators3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance)
	:Textured3DObject(texMgr),
	m_bufPredatorsData(GL_RGB32F)
{
	m_eShaderType = PREDATOR;
	m_pObjectManager = manager;
	load_object(".\\Objects\\Bats\\Bats.obj", false, m_pTextureManager);
	m_pObjectManager->bind_object(this, nbInstance);

	set_position(glm::vec3(-0.0f, 0.0f, 0.0f));
	//set_rotation_angle(-M_PI / 2.0f);
	set_size(1.0f);
	//set_object_type(BAT);
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider_nb_instances_infos(this, nbInstance));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("SizeFactor", 0.01f, 20.0f, 0.1f, get_size()));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("PosX", -500.0f, 500.0f, 1.f, get_x_pos()));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("PosY", -500.0f, 500.0f, 1.f, get_y_pos()));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("PosZ", -500.0f, 500.0f, 1.f, get_z_pos()));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_button("Jump Cam To Predators", jump_cam, (void*)this));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_checkbox("Stick Cam To Predators", &m_bCamSticked));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("Turn Cam around Predators", -3.14f, 3.14f, 0.01f, get_rot_angle()));

	auto tmp = new char[nbInstance * sizeof(glm::vec3)];
	m_bufPredatorsData.setData(nbInstance * sizeof(glm::vec3), tmp);
	delete[] tmp;
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
			m_bufPredatorsData.activate(PREDATORS_BINDING);

			shader->set_var_value("InstanceNumber", (int)sqrt(nbInstance));
			glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)(mesh->get_triangles_count() * 3), GL_UNSIGNED_INT, (void*)0, (GLsizei)nbInstance);

			m_bufPredatorsData.deactivate();
			clean_bindings();
		}
	}
}

void Predators3D::update_positions(const std::vector<glm::vec3>& newPositions)
{
	assert(newPositions.size() <= m_bufPredatorsData.getSize());

	m_bufPredatorsData.updateData(newPositions.data(), 0, newPositions.size() * sizeof(glm::vec3));
	if (newPositions.size() > 0)
		m_v3Position = newPositions[0];
}

