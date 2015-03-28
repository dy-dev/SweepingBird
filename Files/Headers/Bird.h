#pragma once

#include <glm\common.hpp>
#include <Bird3D.h>

namespace SweepingBirds
{
	/* The bird is not a particle itself.
	   The bird attracts other particles: the preys.
	   The bird moves above the ground following random directions on x/z.
	   The bird is always *above* the ground and may never collide with it.
	   */
	class Bird
	{
	public:
		Bird();

		//By default, bird's initial velocity is 1.f toward z
		Bird(float mass, const glm::vec3& initialPosition);

    /**
     * @fn  void Bird::update(const float deltaTime);
     *
     * @brief Updates the given deltaTime.
     *
     * @param deltaTime The delta time.
     */
		void update(const float deltaTime);

    /**
     * @fn  void Bird::set_height(float newHeight);
     *
     * @brief Sets a height.
     *
     * @param newHeight Height of the new.
     */
		void set_height(float newHeight);

    /**
     * @fn  const glm::vec3& Bird::get_position() const;
     *
     * @brief Gets the position.
     *
     * @return  The position.
     */
		const glm::vec3& get_position() const;

    /**
     * @fn  const glm::vec3& Bird::get_velocity() const;
     *
     * @brief Gets the velocity.
     *
     * @return  The velocity.
     */
		const glm::vec3& get_velocity() const;

    /**
     * @fn  float Bird::get_angle() const;
     *
     * @brief Gets the angle.
     *
     * @return  The angle.
     */
		float get_angle() const;

    /**
     * @fn  const Bird3D & Bird::get_bird_3d() const
     *
     * @brief Gets bird 3D.
     *
     * @return  The bird 3D.
     */
		const Bird3D & get_bird_3d() const { return *m_wpBird3D;};

    /**
     * @fn  void Bird::set_bird_3D(Bird3D* const newBird3D)
     *
     * @brief Sets bird 3D.
     *
     * @param [in,out]  newBird3D If non-null, the new bird 3D.
     */
    void set_bird_3D(Bird3D* const newBird3D) { m_wpBird3D = newBird3D; }

    /**
     * @fn  void Bird::update_3D_model();
     *
     * @brief Updates the 3D model.
     */
		void update_3D_model();

	
	private:
		glm::vec3 m_v3Position;
		glm::vec3 m_v3PrevPosition;
		glm::vec3 m_v3Velocity;

		float m_fMass;
		float m_fTimeSinceDirectionChanged;
		float m_fAngle;

		Bird3D* m_wpBird3D;
	};

}