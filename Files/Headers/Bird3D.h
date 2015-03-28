#pragma once
#include <Textured3DObject.h>
namespace SweepingBirds
{
	class Camera;
	class ObjectManager;
	class Bird3D : public Textured3DObject
	{
	public:
    /**
     * @fn  Bird3D::Bird3D();
     *
     * @brief Default constructor.
     */
		Bird3D();

    /**
     * @fn  Bird3D::Bird3D(ObjectManager* manager, TextureManager * texMgr);
     *
     * @brief Constructor.
     *
     * @param [in,out]  manager If non-null, the manager.
     * @param [in,out]  texMgr  If non-null, manager for tex.
     */
		Bird3D(ObjectManager* manager, TextureManager * texMgr);
		~Bird3D();

    /**
     * @fn  void Bird3D::set_speed(float speed)
     *
     * @brief Sets a speed.
     *
     * @param speed The speed.
     */
		void  set_speed(float speed) { m_fSpeed = speed; }

    /**
     * @fn  float * Bird3D::get_speed()
     *
     * @brief Gets the speed.
     *
     * @return  null if it fails, else the speed.
     */
		float * get_speed() { return &m_fSpeed; }

    /**
     * @fn  void Bird3D::set_direction(glm::vec3 Direction)
     *
     * @brief Sets a direction.
     *
     * @param Direction The direction.
     */
		void set_direction(glm::vec3 Direction) { m_v3Direction = Direction; };

    /**
     * @fn  glm::vec3 Bird3D::get_direction()
     *
     * @brief Gets the direction.
     *
     * @return  The direction.
     */
		glm::vec3 get_direction(){ return m_v3Direction; };

    /**
     * @fn  virtual void Bird3D::draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;
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
		float m_fSpeed;

	};

}