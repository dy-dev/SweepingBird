#include "Predator.h"
#include "glm\geometric.hpp"
#include <Bird.h>

using namespace SweepingBirds;

Predator::Predator()
  : m_bird(nullptr), 
    m_scalarK(1.0), 
    m_scalarL(0.f),
    m_mass(0.f)
{

}

Predator::Predator(const float mass, const glm::vec3& initialPosition, const glm::vec3& initialVelocity)
  : m_bird(nullptr), 
    m_mass(mass), 
    m_v3Position(initialPosition), 
    m_v3Velocity(initialVelocity),
    m_v3Direction(glm::normalize(initialVelocity)),
    m_scalarK(1.0), 
    m_scalarL(0.f)
{

}

void Predator::update(const float deltaTime)
{
  //If is following a bird
  if (m_bird != nullptr)
  {
    const glm::vec3& birdPos = m_bird->getPosition();
    glm::vec3 forceToApply = m_scalarK * (1.0f - (m_scalarL / glm::max(glm::distance(m_v3Position, birdPos), 0.001f))) * (birdPos - m_v3Position);

    m_v3Velocity = m_v3Velocity + deltaTime * (forceToApply / m_mass);
    m_v3Position = m_v3Position + deltaTime * m_v3Velocity;
    m_v3Direction = glm::normalize(m_v3Velocity);
  }
  else
  {
    m_v3Position = m_v3Position + deltaTime * m_v3Velocity;
  }
}

void Predator::makeFollow(const Bird* bird)
{
  m_bird = bird;
}

const glm::vec3& Predator::getPosition() const
{
  return m_v3Position;
}

const glm::vec3& Predator::getDirection() const
{
  return m_v3Direction;
}

void Predator::update3DModel()
{
	//m_bPredatorsData.updateData(predatorsPositions.data(), 0, predatorsPositions.size() * sizeof(glm::vec3));
}