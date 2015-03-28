#include "Predator.h"
#include "glm\geometric.hpp"
#include <Bird.h>

using namespace SweepingBirds;

const glm::vec3 Predator::GRAVITY = glm::vec3(0, -9.f, 0);
const glm::vec3 Predator::MAX_VELOCITY = glm::vec3(1000.f);

Predator::Predator()
	: m_bird(nullptr),
	m_fSpringRigidity(1.0),
	m_fSpringLength(0.f),
	m_mass(500.f),
	m_wpPredator3D(nullptr)
{

}

Predator::Predator(const float mass, const glm::vec3& initialPosition, const glm::vec3& initialVelocity)
	: m_bird(nullptr),
	m_mass(mass),
	m_v3Position(initialPosition),
	m_v3Velocity(0),
	m_v3Direction(glm::normalize(initialVelocity)),
	m_fSpringRigidity(1.0),
	m_fSpringLength(0.2f),
	m_wpPredator3D(nullptr)
{

}

void Predator::update(const float deltaTime)
{
	//If is following a bird
	/*if (m_bird != nullptr)
	{
		const glm::vec3& birdPos = m_bird->get_position();
		float distance = glm::distance(m_v3Position, birdPos);
		glm::vec3 birdMinusPred = birdPos - m_v3Position;
		glm::vec3 forceToApply = m_fSpringRigidity * (1.0f - (m_fSpringLength / glm::max(distance, 0.1f))) * birdMinusPred;
		forceToApply += GRAVITY;

		m_v3Velocity = m_v3Velocity + deltaTime * (forceToApply / m_mass);
    //cap velocity
    glm::vec3 tmp = glm::vec3(m_v3Velocity.x > 0 ? 1 : -1, m_v3Velocity.y > 0 ? 1 : -1, m_v3Velocity.z > 0 ? 1 : -1);
    m_v3Velocity = tmp * glm::min(glm::abs(m_v3Velocity), MAX_VELOCITY);
		m_v3Position = m_v3Position + deltaTime * m_v3Velocity;
		m_v3Direction = glm::normalize(m_v3Velocity);
	}
	else
	{
		m_v3Position = m_v3Position + deltaTime * m_v3Velocity;
	}*/
}

void Predator::reset(const glm::vec3& position, const glm::vec3& direction)
{
  m_v3Position = position;
  m_v3Direction = direction;
}

void Predator::make_follow(const Bird* bird)
{
	m_bird = bird;
}

void Predator::set_spring_rigidity(const float rigidity)
{
	m_fSpringRigidity = rigidity;
}
void Predator::set_spring_length(const float length)
{
	m_fSpringLength = length;
}

const glm::vec3& Predator::get_position() const
{
	return m_v3Position;
}


void Predator::set_position(glm::vec3& pos)
{
	m_v3Position = pos;
}

const glm::vec3& Predator::get_direction() const
{
	return m_v3Direction;
}
