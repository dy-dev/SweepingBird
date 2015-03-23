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

		Predators3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance);
		~Predators3D();

		virtual void draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;

		void update_positions(const std::vector<glm::vec3>& newPositions);

	private:
		GPUBuffer m_bufPredatorsData;

	};

}