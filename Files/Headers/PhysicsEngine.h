#pragma once

#include "Bird.h"
#include "Predator.h"
#include <vector>

namespace SweepingBirds
{

	class SceneManager;
class ProgramGUI;

	class PhysicsEngine
	{
	public:
		PhysicsEngine(SceneManager* sceneManager);
		~PhysicsEngine();

		void update(const float deltaTime);
    void set_programGUI(ProgramGUI * programGUI); 

	private:
		void launchPredators();
		void dismissPredators();

		//wp => weak pointer
		SceneManager* m_wpSceneManager;
    ProgramGUI* m_pProgramGUI;

		Bird m_bird;
		std::vector<Predator* > m_vPredators;
		bool m_bPredatorsLaunched;
    bool m_pbResetPredatorsPos;//For debugging purposes

    float m_fPredatorsSpringLength;
    float m_fPredatorsSpringRigidity;

	};
}