#include "Bird.h"
#include "Predator.h"
#include <vector>

class PhysicsEngine
{
public:
  PhysicsEngine();
  ~PhysicsEngine();

  void update(const float deltaTime);

private:
  void launchPredators();
  void dismissPredators();

  Bird m_bird;
  std::vector<Predator* > m_vPredators;
  bool m_bPredatorsLaunched;

};