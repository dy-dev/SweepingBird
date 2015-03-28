#include <TextureManager.h>
#include <Bird3D.h>
#include <ObjectManager.h>
#include <Mesh.h>
#include <ProgramGUI.h>
#include <ShaderProgram.h>
#include <ShaderProgramManager.h>

using namespace SweepingBirds;

Bird3D::Bird3D()
{
}

Bird3D::Bird3D(ObjectManager* manager, TextureManager * texMgr)
	:Textured3DObject(texMgr)
{
	m_eShaderType = BIRD;
	m_pObjectManager = manager;
	load_object(".\\Objects\\Bird\\BeeBird.obj", false, m_pTextureManager);
	set_position(glm::vec3(-0.0f, 0.0f, 0.0f));
	set_size(2.0f);

	m_pObjectManager->add_gui_controller("Birds", m_pObjectManager->generate_slider("SizeFactor", 0.1f, 4.0f, 0.1f, get_size()));
	m_pObjectManager->add_gui_controller("Birds", m_pObjectManager->generate_slider("PosX", -5500.0f, 5500.0f, 1.f, get_x_pos()));
	m_pObjectManager->add_gui_controller("Birds", m_pObjectManager->generate_slider("PosY", -5500.0f, 5500.0f, 1.f, get_y_pos()));
	m_pObjectManager->add_gui_controller("Birds", m_pObjectManager->generate_slider("PosZ", -5500.0f, 5500.0f, 1.f, get_z_pos()));
	m_pObjectManager->add_gui_controller("Birds", m_pObjectManager->generate_button("Jump Cam To Bird", jump_cam, (void*)this));
	m_pObjectManager->add_gui_controller("Birds", m_pObjectManager->generate_button("Jump Cam front To Bird", jump_cam2, (void*)this));
	m_pObjectManager->add_gui_controller("Birds", m_pObjectManager->generate_checkbox("Stick Cam To Bird", &m_bCamSticked));
	m_pObjectManager->add_gui_controller("Birds", m_pObjectManager->generate_slider("Turn Cam around Bird", -3.14f, 3.14f, 0.01f, get_rot_angle()));
	m_pObjectManager->bind_object(this, 1);
}


Bird3D::~Bird3D()
{
}


void Bird3D::draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance)
{
	for each (auto mesh in m_vMeshes)
	{
		auto shader = setup_drawing_space(shaderMgr, mesh, proj, time);
		if (shader != nullptr)
		{
			if (m_bCamSticked)
			{
				m_pObjectManager->set_cam_direction(this,m_fRotAngle);
			}
			glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);
			clean_bindings();
		}
	}
}