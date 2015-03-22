#pragma once
#include <Textured3DObject.h>
namespace SweepingBirds
{
	class ObjectManager;
	class Camera;
	class Predators3D : public Textured3DObject
	{
	public:
		Predators3D();
		Predators3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance);
		~Predators3D();

		virtual void draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;

	};

}