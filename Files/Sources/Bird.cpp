#include "Bird.h"
#include "glm\geometric.hpp"
#include "glm\gtc\random.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include <glm/gtc/quaternion.hpp>
#include <iostream>

#define M_PI_4 3.14159265359f / 4.0f
#define DIRECTION_TIMER_S 3.0f
#define VERTICAL glm::vec3(0.f, 1.f, 0.f)

using namespace SweepingBirds;

Bird::Bird()
{

}

Bird::Bird(float mass, const glm::vec3& initialPosition)
  : m_fMass(mass),
  m_v3Position(initialPosition),
  m_v3PrevPosition(initialPosition),
  m_v3Velocity(glm::vec3(-100.0, 0, 0.f)),
  m_fTimeSinceDirectionChanged(0.0f),
  m_fAngle(0.f),
  m_v3initialDirection(glm::normalize(m_v3Velocity)),
  m_bIsRotating(false),
  m_fLastAngle(0.f),
  m_fNextAngle(0),
  m_fRotDistanceToDo(0),
  m_fElapsedTime(0),
   m_wpBird3D(nullptr)
{

}

//Currently it just moves randomly by changing its
//direction and velocity each 10 seconds \o/
void Bird::update(const float deltaTime)
{ 
  if (m_bIsRotating)
  {
    const float timetomove = 1.f;
    m_fElapsedTime += deltaTime;
    float speed = m_fRotDistanceToDo > 0.f ? 0.2f : -0.2f;
    float finalAngle = glm::angle(m_qDestinationRot);
    float a = m_fElapsedTime / timetomove;// glm::distance((m_fAngle + m_fElapsedTime * speed), m_fLastAngle) / m_fRotDistanceToDo;
    std::cout << "a: " << a << std::endl;
    if (glm::abs(a) <= 1.f)
    {
      glm::quat interpolatedQuat = glm::slerp(m_qInitialRot, m_qDestinationRot, a);

      glm::vec4 tmpDirection = interpolatedQuat * glm::vec4(m_v3initialDirection, 0.0f);

      glm::vec3 newDirection = glm::vec3(tmpDirection.x, tmpDirection.y, tmpDirection.z);
      newDirection = glm::normalize(newDirection);

      //Compute final velocity. Bird never moves on Y Axis
      float newVelocity = glm::linearRand(50.f, 200.f);//Magic numbers to be tested
      m_v3Velocity = glm::vec3(newDirection.x*newVelocity, 0, newDirection.z * newVelocity);

      m_fAngle = glm::angle(interpolatedQuat);

      //std::cout << "New direction with angle: " << m_fAngle << ". Vector: " << newDirection.x << ";" << newDirection.y << ";" << newDirection.z << std::endl;
    }
    else
    {
      m_fAngle = glm::angle(m_qDestinationRot);
      m_fElapsedTime = 0.f;
      m_bIsRotating = false;
      m_qInitialRot = m_qDestinationRot;
    }
  }
  else
    m_fTimeSinceDirectionChanged += deltaTime;

  if (m_fTimeSinceDirectionChanged > DIRECTION_TIMER_S)
  {
    m_bIsRotating = true;
    m_fTimeSinceDirectionChanged = 0.0f;
    const glm::vec3 currentDirection = glm::normalize(m_v3Velocity);

    //To avoid hard turns  we limit the range
    float angleShift = glm::linearRand(-M_PI_4, M_PI_4);
    float lastAngle = glm::angle(m_qInitialRot);
    float nextAngle = lastAngle + angleShift;
    m_fLastAngle = lastAngle;
    m_fAngle = lastAngle;
    m_fRotDistanceToDo = nextAngle - lastAngle;
    m_qDestinationRot = glm::angleAxis(nextAngle, VERTICAL);
    std::cout << "Star rotating with shift of: " << angleShift << " and next angle: " << nextAngle << std::endl;
  }

  m_v3PrevPosition = m_v3Position;
  m_v3Position = m_v3Position + (deltaTime * m_v3Velocity);
}

const glm::vec3& Bird::get_position() const
{
	return m_v3Position;
}

const glm::vec3& Bird::get_velocity() const
{
  return m_v3Velocity;
}

float Bird::get_angle() const
{
  return m_fAngle;
}

void Bird::set_height(float newHeight)
{
  m_v3Position.y = newHeight;
}

void Bird::update_3D_model()
{
	m_wpBird3D->set_position(m_v3Position);
  m_wpBird3D->set_direction(glm::normalize(m_v3Velocity));
  m_wpBird3D->set_angle(m_fAngle);
}
