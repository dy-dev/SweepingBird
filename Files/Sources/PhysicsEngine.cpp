#include "PhysicsEngine.h"

/// The threshold at wich predators start hunting the bird
#define PREDATOR_THRESHOLD_M 15.f

PhysicsEngine::PhysicsEngine()
  : m_bird(2.0f, glm::vec3(0,0,0)),
  m_bPredatorsLaunched(false)
{
  //Basic predator generation for testing purposes
  Predator* a = new Predator(3.f, glm::vec3(0, 0, 0));
  Predator* b = new Predator(2.f, glm::vec3(3, 0, 0));
  Predator* c = new Predator(3.f, glm::vec3(-3, 0, 0));
  
  m_vPredators.push_back(a);
  m_vPredators.push_back(b);
  m_vPredators.push_back(c);
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

  if (m_bird.getPosition().y >= PREDATOR_THRESHOLD_M && !m_bPredatorsLaunched)
  {
    launchPredators();
  }
  else if (m_bPredatorsLaunched && m_bird.getPosition().y < PREDATOR_THRESHOLD_M)
  {
    dismissPredators();
  }

  auto it = m_vPredators.begin();
  for (it; it != m_vPredators.end(); ++it)
  {
    (*it)->update(deltaTime);
  }
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
