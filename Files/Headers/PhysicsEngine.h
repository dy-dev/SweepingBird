#include "Bird.h"
#include "Prey.h"
#include <vector>

class PhysicsEngine
{
public:
  PhysicsEngine();

  void update(const float deltaTime);

private:
  Bird m_bird;
  std::vector<Prey> m_vPreys;

};