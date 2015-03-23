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

		void update(const float deltaTime);
		void set_height(float newHeight);

		const glm::vec3& get_position() const;
		const glm::vec3& get_velocity() const;

		float get_angle() const;

		const Bird3D & get_bird_3d() const { return *m_wpBird3D;};
    void set_bird_3D(Bird3D* const newBird3D) { m_wpBird3D = newBird3D; }
		void update_3D_model();

	
	private:
		glm::vec3 m_v3Position;
		glm::vec3 m_v3PrevPosition;
		glm::vec3 m_v3Velocity;
    glm::vec3 m_v3initialDirection;

    glm::quat m_qInitialRot;
    glm::quat m_qDestinationRot;

		float m_fMass;
		float m_fTimeSinceDirectionChanged;
		float m_fAngle;
    float m_fLastAngle;
    float m_fNextAngle;
    float m_fRotDistanceToDo;
    float m_fElapsedTime;

    bool m_bIsRotating;

		Bird3D* m_wpBird3D;
	};

}