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
		/** @brief The gravity. */
		static const glm::vec3 GRAVITY;
    /** @brief The maximum velocity. */
    static const glm::vec3 MAX_VELOCITY;

    /**
     * @fn  Predator::Predator();
     *
     * @brief Default constructor.
     */
		Predator();

    /**
     * @fn  Predator::Predator(const float mass, const glm::vec3& initialPosition, const glm::vec3& initialVelocity = glm::vec3(0));
     *
     * @brief Constructor.
     *
     * @param mass            The mass.
     * @param initialPosition The initial position.
     * @param initialVelocity The initial velocity.
     */
		Predator(const float mass, const glm::vec3& initialPosition, const glm::vec3& initialVelocity = glm::vec3(0));

    /**
     * @fn  void Predator::update(const float deltaTime);
     *
     * @brief Updates the given deltaTime.
     *
     * @param deltaTime The delta time.
     */
		void update(const float deltaTime);

    /**
     * @fn  void Predator::make_follow(const Bird * const bird);
     *
     * @brief Makes a follow.
     *
     * @param bird  The bird.
     */
		void make_follow(const Bird * const bird);

    /**
     * @fn  void Predator::reset(const glm::vec3& position, const glm::vec3& direction);
     *
     * @brief Resets this object.
     *
     * @param position  The position.
     * @param direction The direction.
     */
    void reset(const glm::vec3& position, const glm::vec3& direction);

    /**
     * @fn  const glm::vec3& Predator::get_position() const;
     *
     * @brief Gets the position.
     *
     * @return  The position.
     */
		const glm::vec3& get_position() const;

    /**
     * @fn  const glm::vec3& Predator::get_direction() const;
     *
     * @brief Gets the direction.
     *
     * @return  The direction.
     */
		const glm::vec3& get_direction() const;

    /**
     * @fn  void Predator::set_spring_rigidity(const float rigidity);
     *
     * @brief Sets spring rigidity.
     *
     * @param rigidity  The rigidity.
     */
		void set_spring_rigidity(const float rigidity);

    /**
     * @fn  void Predator::set_spring_length(const float length);
     *
     * @brief Sets spring length.
     *
     * @param length  The length.
     */
		void set_spring_length(const float length);

    /**
     * @fn  const Predators3D & Predator::get_predator_3d() const
     *
     * @brief Gets predator 3D.
     *
     * @return  The predator 3D.
     */
    const Predators3D & get_predator_3d() const { assert(m_wpPredator3D);  return *m_wpPredator3D; };

    /**
     * @fn  void Predator::set_predators_3D(const Predators3D* newPredators)
     *
     * @brief Sets predators 3D.
     *
     * @param newPredators  The new predators.
     */
    void set_predators_3D(const Predators3D* newPredators){ m_wpPredator3D = newPredators; }

    /**
     * @fn  void Predator::set_position(glm::vec3& pos);
     *
     * @brief Sets a position.
     *
     * @param [in,out]  pos The position.
     */
    void set_position(glm::vec3& pos);

	private:
		const Bird * m_bird;
		glm::vec3 m_v3Position;
		glm::vec3 m_v3Velocity;
		glm::vec3 m_v3Direction;
    glm::mat4 m_matTransform;

		float m_mass;
		float m_fSpringRigidity;
		float m_fSpringLength;

    const Predators3D* m_wpPredator3D;
	};

}