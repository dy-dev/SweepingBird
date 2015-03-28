#include <iostream>

#include <PhysicsEngine.h>
#include <Predator.h>
#include <SceneManager.h>
#include <Textured3DObject.h>
#include <GLFW/glfw3.h>
#include <UtilityToolKit.h>
#include <ProgramGUI.h>
#include <ObjectManager.h>


/// The threshold at wich predators start hunting the bird
#define PREDATOR_THRESHOLD_M 15.f
#define BIRD_OFFSET 10.f

using namespace SweepingBirds;

const unsigned int PhysicsEngine::NB_PREDATORS = 30;

PhysicsEngine::PhysicsEngine(SceneManager* sceneManager)
	: m_wpSceneManager(sceneManager),
	m_Bird(2.0f, glm::vec3(0, 0, -500)),
	m_bPredatorsLaunched(false),
	m_fPredatorsSpringLength(20.0f),
	m_fPredatorsSpringRigidity(0.9f),
	m_pbResetPredatorsPos(false)

{

	//Basic predator generation for testing purposes
	Predator* a = new Predator(3.f, glm::vec3(0, 0, 50));
	Predator* b = new Predator(2.f, glm::vec3(30, 0, 0));
	Predator* c = new Predator(3.f, glm::vec3(-30, 0,20));

	m_vPredators.push_back(a);
	m_vPredators.push_back(b);
	m_vPredators.push_back(c);

	//Link physics object with their 3D representation
	ObjectManager& objectManager = m_wpSceneManager->get_object_manager();

	ClassName<Bird3D> birdName;
	Bird3D* bird3D = dynamic_cast<Bird3D*>(objectManager.get_object(birdName.Name()).first);
	assert(bird3D);
	m_Bird.set_bird_3D(bird3D);

	ClassName<Predators3D> predatorsName;
	m_wpPredators3D = dynamic_cast<Predators3D*>(objectManager.get_object(predatorsName.Name()).first);
	assert(m_wpPredators3D);

	ClassName<Ground3D> groundName;
	Ground3D* ground3D = dynamic_cast<Ground3D*>(objectManager.get_object(groundName.Name()).first);
	assert(ground3D);
	m_Ground.set_ground_3d(ground3D);
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
	m_pProgramGUI->add_gui_element(name, infos);

	auto infos3 = new GUIInfos(name, -50.0f, 50.0f, 0.1f);
	infos->min = 0.f;
	infos->max = 5.f;
	infos->step = 0.1f;
	infos->var.push_back(std::make_pair("Spring rigidity", &(m_fPredatorsSpringRigidity)));
	m_pProgramGUI->add_gui_element(name, infos3);

	auto infos2 = new GUIInfos(name, CHECKBOX);
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

	float MountainFrequence = m_Ground.get_mountain_frequency();
	float MaxMountainHeight = m_Ground.get_ground_height();
	int freq = int(MountainFrequence);
	if (freq == 0)
		freq = 1;

	glm::vec3 birdPos = m_Bird.get_position();
	float tempx = birdPos.x / (10 * freq);// - Time;
	float tempz = birdPos.z / (10 * freq);
	float birdHeight = 0;
	/*
	birdHeight = MaxMountainHeight*(cos(tempx)*cos(2.0*tempx)*sin(4.0*tempz) + sin(tempz + 1.5)*sin(2.0*tempz)*cos(tempx*8.0));
	birdHeight += MaxMountainHeight*(sin(tempz / 5.0)*cos(3.0*tempx) + sin(tempx)*sin(5 * tempz));
	birdHeight += MaxMountainHeight*(cos(tempz + 1.5)*sin(tempz)*cos(9.0*tempx) + cos(tempx + 1.5)*cos(tempz / 5.0)*sin(tempx*5.0));
	*/
	birdHeight = 2 * MaxMountainHeight*m_Ground.Total(abs(birdPos.x), abs(birdPos.z));

	birdHeight += BIRD_OFFSET;

	m_Bird.set_height(birdHeight);
	m_Bird.update_3D_model();

	for (auto it = m_vPredators.begin(); it != m_vPredators.end(); ++it)
	{
		//(*it)->set_spring_length(m_fPredatorsSpringLength);
		//(*it)->set_spring_rigidity(m_fPredatorsSpringRigidity);
		(*it)->update(deltaTime);
	}

	/*------ UPDATE GRAPHICS ---------- */

	//May be optimized
	std::vector<glm::vec3> predatorsPositions;
	std::vector<glm::vec3> predatorsDirections;

	for each (auto pred in m_vPredators)
	{
		glm::vec3 finalPos = pred->get_position();
		if (m_pbResetPredatorsPos)
			finalPos = m_Bird.get_position();

		predatorsPositions.push_back(finalPos);
		predatorsDirections.push_back(pred->get_direction());
	}
	

	m_wpPredators3D->update_positions(predatorsPositions);
}

void PhysicsEngine::launch_predators()
{
	if (m_bPredatorsLaunched)
		return;

	for each (auto predator in m_vPredators)
	{
		predator->make_follow(&m_Bird);
	}

	m_bPredatorsLaunched = true;
}

void PhysicsEngine::dismiss_predators()
{
	if (!m_bPredatorsLaunched)
		return;

	for each (auto predator in m_vPredators)
	{
		predator->make_follow(nullptr);
	}
	m_bPredatorsLaunched = false;
}
