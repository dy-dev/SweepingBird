#include <iostream>

#include <PhysicsEngine.h>
#include <Predator.h>
#include <SceneManager.h>
#include <Textured3DObject.h>
#include <GLFW/glfw3.h>
#include <UtilityToolKit.h>
#include <ProgramGUI.h>


/// The threshold at wich predators start hunting the bird
#define PREDATOR_THRESHOLD_M 15.f
#define BIRD_OFFSET 2.f

using namespace SweepingBirds;

PhysicsEngine::PhysicsEngine(SceneManager* sceneManager)
  : m_wpSceneManager(sceneManager),
  m_Bird(2.0f, glm::vec3(0, 0, 0)),
  m_bPredatorsLaunched(false)
  
{
  //Basic predator generation for testing purposes
  Predator* a = new Predator(3.f, glm::vec3(0, 0, 0));
  Predator* b = new Predator(2.f, glm::vec3(3, 0, 0));
  Predator* c = new Predator(3.f, glm::vec3(-3, 0, 0));
  
  m_vPredators.push_back(a);
  m_vPredators.push_back(b);
  m_vPredators.push_back(c);

  const int MAX_PREDATORS = 20;
  m_wpSceneManager->setup_predators(MAX_PREDATORS);
}

PhysicsEngine::~PhysicsEngine()
{
  //Destroy predators's list
  auto it = m_vPredators.begin();
  for (it; it != m_vPredators.end(); ++it)
  {
    delete (*it);
    (*it) = nullptr;
  }

  m_vPredators.clear();
}

void PhysicsEngine::update(const float deltaTime)
{
  m_Bird.update(deltaTime);


 // if (m_bird.getPosition().y >= PREDATOR_THRESHOLD_M && !m_bPredatorsLaunched)
 // {
    launchPredators();
 // }
 // else if (m_bPredatorsLaunched && m_bird.getPosition().y < PREDATOR_THRESHOLD_M)
 // {
  //  dismissPredators();
 // }

  float freq = m_Ground.getMountainFrequency();
  if (freq == 0)
  {
    freq = 0.001;
  }
 
  float MaxMountainHeight = m_Ground.getGroundHeight();
  float birdHeight = MaxMountainHeight*(cos(m_pProgramGUI->get_time() + 1.5)*sin(m_pProgramGUI->get_time()*5.0));

  std::cerr << " birdHeight : " << birdHeight << std::endl;
  birdHeight += BIRD_OFFSET;
  
  m_Bird.setHeight(birdHeight);
  m_Bird.update3DModel();

  auto it = m_vPredators.begin();
  for (it; it != m_vPredators.end(); ++it)
  {
    (*it)->update(deltaTime);
  }

  /*------ UPDATE GRAPHICS ---------- */

  //May be optimized
  it = m_vPredators.begin();
  std::vector<glm::vec3> predatorsPositions;
  std::vector<glm::vec3> predatorsDirections;
  for (it; it != m_vPredators.end(); ++it)
  {
    predatorsPositions.push_back((*it)->getPosition());
    predatorsDirections.push_back((*it)->getDirection());
  }

  //m_wpSceneManager->updatePredators(predatorsPositions, predatorsDirections);

}

void PhysicsEngine::launchPredators()
{
  auto it = m_vPredators.begin();
  for (it; it != m_vPredators.end(); ++it)
  {
    (*it)->makeFollow(&m_Bird);
  }
  m_bPredatorsLaunched = true;
}

void PhysicsEngine::dismissPredators()
{
  auto it = m_vPredators.begin();
  for (it; it != m_vPredators.end(); ++it)
  {
    (*it)->makeFollow(nullptr);
  }
  m_bPredatorsLaunched = false;
}
