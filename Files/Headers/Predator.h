#pragma once

#include <glm\common.hpp>
#include <Predators3D.h>

/* A Predator is a particle following a bird in
   a "Hook Style".
   See http://igm.univ-mlv.fr/~lnoel/index.php?section=teaching&teaching=physic_engines&teaching_section=tds&td=td3#hook
   */

namespace SweepingBirds
{
	class Bird;

	class Predator
	{
	public:
		static const glm::vec3 GRAVITY;

		Predator();
		Predator(const float mass, const glm::vec3& initialPosition, const glm::vec3& initialVelocity = glm::vec3(0));

		void update(const float deltaTime);
		void make_follow(const Bird * const bird);

		const glm::vec3& get_position() const;
		const glm::vec3& get_direction() const;
		void set_spring_rigidity(const float rigidity);
		void set_spring_length(const float length);

		const Predators3D & get_predator_3d() const { return m_Predator3D; };

		void update3DModel();

	private:
		const Bird * m_bird;
		glm::vec3 m_v3Position;
		glm::vec3 m_v3Velocity;
		glm::vec3 m_v3Direction;

		float m_mass;
		float m_fSpringRigidity;
		float m_fSpringLength;

		Predators3D m_Predator3D;
	};

}