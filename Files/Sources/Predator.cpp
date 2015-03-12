#include "Predator.h"

Predator::Predator()
{

}

Predator::Predator(const float mass, const glm::vec3& initialPosition, const glm::vec3& initialVelocity)
  : m_mass(mass), m_v3Position(initialPosition), m_v3Velocity(initialVelocity)
{

}

void Predator::update(const float deltaTime)
{

}

void Predator::makeFollow(const Bird& bird)
{

}