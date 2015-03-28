#pragma once
#include <Textured3DObject.h>
namespace SweepingBirds
{
	class Camera;
	class ObjectManager;
	class Ground3D : public Textured3DObject
	{
	public:

    /**
     * @fn  Ground3D::Ground3D();
     *
     * @brief Default constructor.
     */
		Ground3D();

    /**
     * @fn  Ground3D::Ground3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance);
     *
     * @brief Constructor.
     *
     * @param [in,out]  manager If non-null, the manager.
     * @param [in,out]  texMgr  If non-null, manager for tex.
     * @param nbInstance        The nb instance.
     */
		Ground3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance);
		~Ground3D();

    /**
     * @fn  const float& Ground3D::get_height() const
     *
     * @brief Gets the height.
     *
     * @return  The height.
     */
		const float& get_height() const { return m_fHeight; }

    /**
     * @fn  void Ground3D::set_height(float height)
     *
     * @brief Sets a height.
     *
     * @param height  The height.
     */
		void set_height(float height) { m_fHeight = height; }

    /**
     * @fn  const float& Ground3D::get_frequency() const
     *
     * @brief Gets the frequency.
     *
     * @return  The frequency.
     */
    const float& get_frequency() const { return m_fMountainFrequency; }

    /**
     * @fn  virtual void Ground3D::draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;
     *
     * @brief Draws.
     *
     * @param [in,out]  shaderMgr Manager for shader.
     * @param proj                The project.
     * @param time                The time.
     * @param nbInstance          The nb instance.
     */
		virtual void draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;

	private:
		float m_fHeight;
		float m_fMountainFrequency;
		float m_iPatchSize;
	};

}