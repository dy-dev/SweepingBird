#include "glm\common.hpp"

class Bird;

/* A Predator is a particle following a bird in
   a "Hook Style".
   See http://igm.univ-mlv.fr/~lnoel/index.php?section=teaching&teaching=physic_engines&teaching_section=tds&td=td3#hook
*/
class Predator
{
public:
  Predator();
  Predator(const float mass, const glm::vec3& initialPosition, const glm::vec3& initialVelocity = glm::vec3(0));

  void update(const float deltaTime);
  void makeFollow(const Bird * const bird);

  const glm::vec3& getPosition() const;
  const glm::vec3& getDirection() const;

private:
  const Bird * m_bird;
  glm::vec3 m_v3Position;
  glm::vec3 m_v3Velocity;
  glm::vec3 m_v3Direction;

  float m_mass;
  float m_scalarK;
  float m_scalarL;
};