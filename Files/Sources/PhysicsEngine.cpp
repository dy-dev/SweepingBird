#include <iostream>

#include <PhysicsEngine.h>
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
  m_bird(2.0f, glm::vec3(0, 0, 0)),
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
  m_bird.update(deltaTime);


 // if (m_bird.getPosition().y >= PREDATOR_THRESHOLD_M && !m_bPredatorsLaunched)
 // {
    launchPredators();
 // }
 // else if (m_bPredatorsLaunched && m_bird.getPosition().y < PREDATOR_THRESHOLD_M)
 // {
  //  dismissPredators();
 // }

  Textured3DObject* ground = m_wpSceneManager->getGround();
  Textured3DObject* bird = m_wpSceneManager->getBird();
  
  float freq = *ground->get_radius_spacing();
  if (freq == 0)
  {
    freq = 0.001;
  }
 
  float MaxMountainHeight = *ground->get_height();
  float birdHeight = MaxMountainHeight*(cos(m_pProgramGUI->get_time() + 1.5)*sin(m_pProgramGUI->get_time()*5.0));

  std::cerr << " birdHeight : " << birdHeight << std::endl;
  birdHeight += BIRD_OFFSET;
  
  m_bird.setHeight(birdHeight);
  bird->set_mock_pos(m_bird.getTranslation());

  auto it = m_vPredators.begin();
  for (it; it != m_vPredators.end(); ++it)
  {
    (*it)->update(deltaTime);
  }

  /*------ UPDATE GRAPHICS ---------- */

  m_wpSceneManager->updateBird(m_bird.getPosition(), m_bird.getAngle());

  //May be optimized
  it = m_vPredators.begin();
  std::vector<glm::vec3> predatorsPositions;
  std::vector<glm::vec3> predatorsDirections;
  for (it; it != m_vPredators.end(); ++it)
  {
    predatorsPositions.push_back((*it)->getPosition());
    predatorsDirections.push_back((*it)->getDirection());
  }

  m_wpSceneManager->updatePredators(predatorsPositions, predatorsDirections);

}

void PhysicsEngine::launchPredators()
{
  auto it = m_vPredators.begin();
  for (it; it != m_vPredators.end(); ++it)
  {
    (*it)->makeFollow(&m_bird);
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
