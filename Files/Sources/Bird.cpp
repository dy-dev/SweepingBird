#include "Bird.h"
#include "glm\geometric.hpp"
#include "glm\gtc\random.hpp"
#include "glm/gtx/rotate_vector.hpp"

#define M_PI_4 3.14159265359f / 4.0f
#define DIRECTION_TIMER_S 10.0f
#define VERTICAL glm::vec3(0.f, 1.f, 0.f)

Bird::Bird()
{

}

Bird::Bird(float mass, const glm::vec3& initialPosition)
 : m_mass(mass),
   m_v3Position (initialPosition),
   m_timeSinceDirectionChanged(0.0f)
{

}

//FInal idea: the bird is a following a point evolving randomly on the
//ground. The bird is attached to this point as via a hook.
//It allows to have inertia when it turns etc.

//Currently it just moves randomly by changing its
//direction and velocity each 10 seconds \o/
void Bird::update(const float deltaTime)
{
  m_timeSinceDirectionChanged += deltaTime;

  if (m_timeSinceDirectionChanged > DIRECTION_TIMER_S)
  {
    m_timeSinceDirectionChanged = 0.0f;
    const glm::vec3 currentDirection = glm::normalize(m_v3Velocity);

    //To avoid hard turns  we limit the range
    float movingAngle = glm::linearRand(-M_PI_4, M_PI_4);
    glm::vec3 newDirection = glm::rotate(currentDirection, movingAngle, VERTICAL);

    //Compute final velocity. Bird never moves on Y Axis
    glm::vec2 newVelocity = glm::linearRand(glm::vec2(1.f), glm::vec2(4.f));//Magic numbers to be tested
    m_v3Velocity = newDirection * glm::vec3(newVelocity.x, 0.0f, newVelocity.y);
  }

  m_v3Position = m_v3Position + deltaTime * m_v3Velocity;
  //TO DO : add terrain height offset in y position
}

const glm::vec3& Bird::getPosition() const
{
  return m_v3Position;
}

const glm::vec3& Bird::getVelocity() const
{
  return m_v3Velocity;
}