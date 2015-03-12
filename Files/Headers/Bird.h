#include "glm\common.hpp"

/* The bird is not a particle itself.
   The bird attracts other particles: the preys.
   The bird moves above the ground following random directions.
*/
class Bird
{
public:
  Bird();
  explicit Bird(const glm::vec3& initialPosition);

  void update(const float deltaTime);

private:
  glm::vec3 m_v3Position;
  glm::vec3 m_v3Velocity;

};