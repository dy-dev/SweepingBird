#pragma once

#include <Bird.h>
#include <Ground.h>
#include <vector>

namespace SweepingBirds
{
	class Predator;
	class SceneManager;
	class ProgramGUI;

	class PhysicsEngine
	{
	public:
		PhysicsEngine(SceneManager* sceneManager);
		~PhysicsEngine();

		void update(const float deltaTime);
		void set_programGUI(ProgramGUI * programGUI){ m_pProgramGUI = programGUI; }

	private:
		void launchPredators();
		void dismissPredators();

		//wp => weak pointer
		SceneManager* m_wpSceneManager;
		ProgramGUI* m_pProgramGUI;

		Bird m_Bird;
		std::vector<Predator* > m_vPredators;
		Ground m_Ground;
		bool m_bPredatorsLaunched;

	};
}