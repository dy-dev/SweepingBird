#include <SkyBoxSweepingBird.h>
#include <ObjectManager.h>
#include <Mesh.h>
#include <ShaderProgram.h>
#include <ShaderProgramManager.h>
#include <Camera.h>

using namespace SweepingBirds;

SkyBoxSweepingBird::SkyBoxSweepingBird(ObjectManager* manager, TextureManager * texMgr)
	:Textured3DObject(texMgr)
{
	m_eShaderType = SKYBOX;
	m_pObjectManager = manager;

	load_object(".\\Objects\\SkyBox\\SkyBox.obj", false, m_pTextureManager);
	m_pObjectManager->bind_object(this, 1);

	set_position(glm::vec3(0.0, 0.0, -5000.0));
	set_size(1.0f);

	m_pObjectManager->add_gui_controller("Skybox", m_pObjectManager->generate_slider("SizeFactor", 0.001f, 2.0f, 0.1f, get_size()));
	m_pObjectManager->add_gui_controller("Skybox", m_pObjectManager->generate_slider("PosX", -1500.0f, 1500.0f, 1.f, get_x_pos()));
	m_pObjectManager->add_gui_controller("Skybox", m_pObjectManager->generate_slider("PosY", -1500.0f, 1500.0f, 1.f, get_y_pos()));
	m_pObjectManager->add_gui_controller("Skybox", m_pObjectManager->generate_slider("PosZ", -5000.0f, -1500.0f, 1.f, get_z_pos()));
}


SkyBoxSweepingBird::~SkyBoxSweepingBird()
{
}


void SkyBoxSweepingBird::draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance)
{
	for each (auto mesh in m_vMeshes)
	{
		auto shader = setup_drawing_space(shaderMgr, mesh, proj, time);
		if (shader != nullptr)
		{
			GLint OldCullFaceMode;
			glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
			GLint OldDepthFuncMode;
			glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

			glCullFace(GL_FRONT);
			glDepthFunc(GL_LEQUAL);

			auto cam = m_pObjectManager->get_Camera(); 
			glm::mat4 Model;
			auto ModelTranslated = glm::translate(Model, cam->GetEye());
			shader->set_var_value("GroundTranslation", glm::value_ptr(ModelTranslated));

			glDrawElements(GL_TRIANGLES, mesh->get_triangles_count() * 3, GL_UNSIGNED_INT, (void*)0);

			clean_bindings();

			glCullFace(OldCullFaceMode);
			glDepthFunc(OldDepthFuncMode);
		}
	}
}
