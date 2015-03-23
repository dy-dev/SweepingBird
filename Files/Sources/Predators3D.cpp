#include <Predators3D.h>
#include <ShaderProgram.h>
#include <Mesh.h>
#include <ObjectManager.h>
#include <ShaderProgramManager.h>
#include <glfw\glfw3.h>

using namespace SweepingBirds;

const GLuint Predators3D::PREDATORS_BINDING = GL_TEXTURE5;
const GLuint Predators3D::MATRIX_TRANSFORM = 3;

Predators3D::Predators3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance)
	:Textured3DObject(texMgr),
  m_uiNbInstances(nbInstance)

{
	m_eShaderType = PREDATOR;
	m_pObjectManager = manager;
	load_object(".\\Objects\\Bats\\Bats.obj", false, m_pTextureManager);
	m_pObjectManager->bind_object(this, nbInstance);

	set_position(glm::vec3(-0.0f, 0.0f, 0.0f));
	//set_rotation_angle(-M_PI / 2.0f);
	set_size(1.0f);
	//set_object_type(BAT);
  m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider_nb_instances_infos("Predators", m_uiNbInstances));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("SizeFactor", 0.01f, 20.0f, 0.1f, get_size()));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("PosX", -500.0f, 500.0f, 1.f, get_x_pos()));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("PosY", -500.0f, 500.0f, 1.f, get_y_pos()));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_slider("PosZ", -500.0f, 500.0f, 1.f, get_z_pos()));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_button("Jump Cam To Predators", jump_cam, (void*)this));
	m_pObjectManager->add_gui_controller("Predators", m_pObjectManager->generate_checkbox("Stick Cam To Predators", &m_bCamSticked));

  init_instanced_buffer();
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
			shader->set_var_value("InstanceNumber", (int)sqrt(nbInstance));
			glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)(mesh->get_triangles_count() * 3), GL_UNSIGNED_INT, (void*)0, (GLsizei)nbInstance);
			clean_bindings();
		}
	}
}

void Predators3D::update_transformation(const std::vector<glm::mat4>& transformMatrices)
{
  assert(transformMatrices.size() <= m_uiNbInstances);

  glBindBuffer(GL_ARRAY_BUFFER, m_uiTransformMatricesBuffer);
  glm::mat4 * matrices = (glm::mat4 *)glMapBuffer(GL_ARRAY_BUFFER,
    GL_WRITE_ONLY);
  memcpy(matrices, transformMatrices.data(), transformMatrices.size() * sizeof(glm::mat4));
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Predators3D::init_instanced_buffer()
{
  glGenBuffers(1, &m_uiTransformMatricesBuffer);

  glBindBuffer(GL_ARRAY_BUFFER, m_uiTransformMatricesBuffer);
  glBufferData(GL_ARRAY_BUFFER, m_uiNbInstances*sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

  for each (auto mesh in m_vMeshes)
  {
    glBindVertexArray(mesh->get_vao());
    glBindBuffer(GL_ARRAY_BUFFER, m_uiTransformMatricesBuffer);

    // Likewise, we can do the same with the model matrix. Note that a
    // matrix input to the vertex shader consumes N consecutive input
    // locations, where N is the number of columns in the matrix. So...
    // we have four vertex attributes to set up.
    // Loop over each column of the matrix...
    for (int i = 0; i < 4; i++)
    {
      // Set up the vertex attribute
      glVertexAttribPointer(MATRIX_TRANSFORM + i,              // Location
        4, GL_FLOAT, GL_FALSE,       // vec4
        sizeof(glm::mat4),                // Stride
        (void *)(sizeof(glm::vec4) * i)); // Start offset
      // Enable it
      glEnableVertexAttribArray(MATRIX_TRANSFORM + i);
      // Make it instanced
      glVertexAttribDivisor(MATRIX_TRANSFORM + i, 1);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
}
