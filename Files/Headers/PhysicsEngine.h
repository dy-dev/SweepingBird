#include "Bird.h"
#include "Predator.h"
#include <vector>

class SceneManager;

class PhysicsEngine
{
public:
  PhysicsEngine(SceneManager* sceneManager);
  ~PhysicsEngine();

  void update(const float deltaTime);

private:
  void launchPredators();
  void dismissPredators();

  //wp => weak pointer
  SceneManager* m_wpSceneManager;

  Bird m_bird;
  std::vector<Predator* > m_vPredators;
  bool m_bPredatorsLaunched;

};