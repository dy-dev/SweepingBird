#include "Bird.h"

Bird::Bird()
{

}

Bird::Bird(const glm::vec3& initialPosition)
 : m_v3Position (initialPosition)
{

}

void Bird::update(const float deltaTime)
{

}

const glm::vec3& Bird::getPosition() const
{
  return m_v3Position;
}

const glm::vec3& Bird::getVelocity() const
{
  return m_v3Velocity;
}