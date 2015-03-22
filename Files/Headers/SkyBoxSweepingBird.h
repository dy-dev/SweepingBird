#pragma once
#include <Textured3DObject.h>
namespace SweepingBirds
{
	class Camera;
	class SkyBoxSweepingBird : public Textured3DObject
	{
	public:
		SkyBoxSweepingBird();
		SkyBoxSweepingBird(ObjectManager* manager, TextureManager * texMgr);
		~SkyBoxSweepingBird();

		virtual void draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;
	};
}