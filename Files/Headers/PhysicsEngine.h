#include "Bird.h"
#include "Predator.h"
#include <vector>

class PhysicsEngine
{
public:
  PhysicsEngine();

  void update(const float deltaTime);

private:
  Bird m_bird;
  std::vector<Predator> m_vPreys;

};