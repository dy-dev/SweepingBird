#include "glm\common.hpp"
#include "Bird.h"

class Predator
{
public:
  Predator();
  Predator(const float mass, const glm::vec3& initialPosition, const glm::vec3& initialVelocity = glm::vec3(0));

  void update(const float deltaTime);
  void makeFollow(const Bird& bird);

private:
  const Bird* m_bird;
  glm::vec3 m_v3Position;
  glm::vec3 m_v3Velocity;

  float m_mass;

  //See http://igm.univ-mlv.fr/~lnoel/index.php?section=teaching&teaching=physic_engines&teaching_section=tds&td=td3#hook
  float m_scalarK;
  float m_scalarL;
};