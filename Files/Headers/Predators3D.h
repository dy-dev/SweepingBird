#pragma once
#include <Textured3DObject.h>
#include <GPUBuffer.h>

namespace SweepingBirds
{
	class ObjectManager;
	class Camera;
	class Predators3D : public Textured3DObject
	{
  public:		
    static const GLuint PREDATORS_BINDING;
    static const GLuint MATRIX_TRANSFORM;


		Predators3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance);
		~Predators3D();

		virtual void draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;

    void update_transformation(const std::vector<glm::mat4>& transformMatrices);

  private:
    void init_instanced_buffer();


    unsigned int m_uiNbInstances;
    GLuint m_uiTransformMatricesBuffer;
	};

}