#pragma once
#include <Textured3DObject.h>
namespace SweepingBirds
{
	class Camera;
	class SkyBoxSweepingBird : public Textured3DObject
	{
	public:
    /**
     * @fn  SkyBoxSweepingBird::SkyBoxSweepingBird();
     *
     * @brief Default constructor.
     */
		SkyBoxSweepingBird();
    /**
     * @fn  SkyBoxSweepingBird::SkyBoxSweepingBird(ObjectManager* manager, TextureManager * texMgr);
     *
     * @brief Constructor.
     *
     * @param [in,out]  manager If non-null, the manager.
     * @param [in,out]  texMgr  If non-null, manager for tex.
     */
		SkyBoxSweepingBird(ObjectManager* manager, TextureManager * texMgr);
		~SkyBoxSweepingBird();

    /**
     * @fn  virtual void SkyBoxSweepingBird::draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;
     *
     * @brief Draws.
     *
     * @param [in,out]  shaderMgr Manager for shader.
     * @param proj                The project.
     * @param time                The time.
     * @param nbInstance          The nb instance.
     */
		virtual void draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;
	};
}