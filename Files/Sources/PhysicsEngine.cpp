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
	m_bPredatorsLaunched(false),
	m_fPredatorsSpringLength(50.0f),
	m_fPredatorsSpringRigidity(3.f),
	m_pbResetPredatorsPos(false)

{

  //Basic predator generation for testing purposes
  Predator* a = new Predator(3.f, glm::vec3(0, 0, 50));
  Predator* b = new Predator(2.f, glm::vec3(30, 0, 0));
  Predator* c = new Predator(3.f, glm::vec3(-30, 0, 0));
  
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

void PhysicsEngine::set_programGUI(ProgramGUI * programGUI)
{
	m_pProgramGUI = programGUI;

	std::string name = "Predators";
	auto infos = new GUIInfos(name, -50.0f, 50.0f, 0.1f);
	infos->min = 0.1f;
	infos->max = 100.f;
	infos->step = 0.5f;
	infos->var.push_back(std::make_pair("Spring length", &(m_fPredatorsSpringLength)));
	infos->var.push_back(std::make_pair("Spring rigidity", &(m_fPredatorsSpringRigidity)));
	m_pProgramGUI->add_gui_element(name, infos);

	auto infos2 = new GUIInfos(name);
	infos2->check_adress = &m_pbResetPredatorsPos;
	m_pProgramGUI->add_gui_element(name, infos2);
}

void PhysicsEngine::update(const float deltaTime)
{
	m_Bird.update(deltaTime);


	// if (m_bird.get_position().y >= PREDATOR_THRESHOLD_M && !m_bPredatorsLaunched)
	// {
	launch_predators();
	// }
	// else if (m_bPredatorsLaunched && m_bird.get_position().y < PREDATOR_THRESHOLD_M)
	// {
	//  dismiss_predators();
	// }

	float freq = m_Ground.getMountainFrequency();
	if (freq == 0)
	{
		freq = 0.001;
	}

	float MaxMountainHeight = m_Ground.getGroundHeight();
	float birdHeight = MaxMountainHeight*(cos(m_wpSceneManager->get_time() + 1.5)*sin(m_wpSceneManager->get_time()*5.0));

	birdHeight += BIRD_OFFSET;

	m_Bird.set_height(birdHeight);
	m_Bird.update3DModel();

	auto it = m_vPredators.begin();
	for (it; it != m_vPredators.end(); ++it)
	{
		(*it)->set_spring_length(m_fPredatorsSpringLength);
		(*it)->set_spring_rigidity(m_fPredatorsSpringRigidity);
		(*it)->update(deltaTime);
	}

	/*------ UPDATE GRAPHICS ---------- */

	//May be optimized
	it = m_vPredators.begin();
	std::vector<glm::vec3> predatorsPositions;
	std::vector<glm::vec3> predatorsDirections;
	for each (auto pred in m_vPredators)
	{
		//tricks for position stuff
		glm::vec3 finalPos = pred->get_position();
		finalPos.x -= m_Bird.get_position().x;
		finalPos.z -= m_Bird.get_position().z;

		if (m_pbResetPredatorsPos)
			finalPos = glm::vec3(0);

		predatorsPositions.push_back(finalPos);
		predatorsDirections.push_back((*it)->get_direction());
	}

	//m_wpSceneManager->updatePredators(predatorsPositions, predatorsDirections);

}

void PhysicsEngine::launch_predators()
{
	for each (auto predator in m_vPredators)
	{
		predator->make_follow(&m_Bird);
	}

	m_bPredatorsLaunched = true;
}

void PhysicsEngine::dismiss_predators()
{
	for each (auto predator in m_vPredators)
	{
		predator->make_follow(nullptr);
	}
	m_bPredatorsLaunched = false;
}
