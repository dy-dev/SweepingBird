#include "Prey.h"

Prey::Prey()
{

}

Prey::Prey(const float mass, const glm::vec3& initialPosition, const glm::vec3& initialVelocity)
  : m_mass(mass), m_v3Position(initialPosition), m_v3Velocity(initialVelocity)
{

}

void Prey::update(const float deltaTime)
{

}

void Prey::makeFollow(const Bird& bird)
{

}