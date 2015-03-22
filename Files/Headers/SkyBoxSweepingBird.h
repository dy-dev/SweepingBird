#pragma once
#include <Textured3DObject.h>
namespace SweepingBirds
{
	class ObjectManager;
	class SkyBoxSweepingBird : public Textured3DObject
	{
	public:
		SkyBoxSweepingBird();
		SkyBoxSweepingBird(ObjectManager* manager, TextureManager * texMgr);
		~SkyBoxSweepingBird();

		virtual void draw(ShaderProgram& shader, glm::mat4 proj, glm::mat4 wtv, float time, int nbInstance) override;

	};

}