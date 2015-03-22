#pragma once
#include <Textured3DObject.h>
namespace SweepingBirds
{
	class ObjectManager;
	class Predators3D : public Textured3DObject
	{
	public:
		Predators3D();
		Predators3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance);
		~Predators3D();

		virtual void draw(ShaderProgram& shader, glm::mat4 proj, glm::mat4 wtv, float time, int nbInstance) override;

	};

}