#include "glm\common.hpp"
#include "Bird.h"

class Prey
{
public:
  Prey();
  Prey(const float mass, const glm::vec3& initialPosition, const glm::vec3& initialVelocity = glm::vec3(0));

  void update(const float deltaTime);
  void makeFollow(const Bird& bird);

private:
  glm::vec3 m_v3Position;
  glm::vec3 m_v3Velocity;

  float m_mass;
};