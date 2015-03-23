#pragma once

#include <Bird.h>
#include <Ground.h>
#include <vector>

namespace SweepingBirds
{
	class Predator;
	class SceneManager;
	class ProgramGUI;
	class Predators3D;

	class PhysicsEngine
	{
	public:
		static const unsigned int NB_PREDATORS;

		PhysicsEngine(SceneManager* sceneManager);
		~PhysicsEngine();

		void update(const float deltaTime);
		void set_programGUI(ProgramGUI * programGUI);

	private:
		void launch_predators();
		void dismiss_predators();

		//wp => weak pointer
		SceneManager* m_wpSceneManager;
		ProgramGUI* m_pProgramGUI;
		Predators3D* m_wpPredators3D;

		Bird m_Bird;
		std::vector<Predator* > m_vPredators;
		Ground m_Ground;
		bool m_bPredatorsLaunched;
		bool m_pbResetPredatorsPos;//For debugging purposes

		float m_fPredatorsSpringLength;
		float m_fPredatorsSpringRigidity;

	};
}