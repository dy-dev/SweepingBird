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
		void setHeight(float newHeight);

		const glm::vec3& getPosition() const;
		const glm::vec3& getVelocity() const;

		float getAngle() const;

		const Bird3D & get_bird_3d() const { return m_Bird3D;};
		void update3DModel();
	private:
		glm::vec3 m_v3Position;
		glm::vec3 m_v3PrevPosition;
		glm::vec3 m_v3Velocity;

		float m_fMass;
		float m_fTimeSinceDirectionChanged;
		float m_fAngle;

		Bird3D m_Bird3D;
	};

}