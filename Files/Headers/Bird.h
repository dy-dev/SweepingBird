#include "glm\common.hpp"

/* The bird is not a particle itself.
   The bird attracts other particles: the preys.
   The bird moves above the ground following random directions on x/z.
   The bird is always *above* the ground and may never collide with it.
*/
class Bird
{
public:
  Bird();
  Bird(float mass, const glm::vec3& initialPosition);

  void update(const float deltaTime);

  const glm::vec3& getPosition() const;
  const glm::vec3& getVelocity() const;

private:
  glm::vec3 m_v3Position;
  glm::vec3 m_v3Velocity;
 
  float m_mass;
  float m_timeSinceDirectionChanged;

};