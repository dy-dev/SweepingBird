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
    /**
     * @fn  static void PhysicsEngine::jump_preds(void *obj, bool stick = false);
     *
     * @brief Jump preds.
     *
     * @param [in,out]  obj If non-null, the object.
     * @param stick         true to stick.
     */
		static void jump_preds(void *obj, bool stick = false);

	public:
		/** @brief The nb predators. */
		static const unsigned int NB_PREDATORS;

    /**
     * @fn  PhysicsEngine::PhysicsEngine(SceneManager* sceneManager);
     *
     * @brief Constructor.
     *
     * @param [in,out]  sceneManager  If non-null, manager for scene.
     */
		PhysicsEngine(SceneManager* sceneManager);
		~PhysicsEngine();

    /**
     * @fn  void PhysicsEngine::update(const float deltaTime);
     *
     * @brief Updates the given deltaTime.
     *
     * @param deltaTime The delta time.
     */
		void update(const float deltaTime);

    /**
     * @fn  void PhysicsEngine::set_programGUI(ProgramGUI * programGUI);
     *
     * @brief Sets program graphical user interface.
     *
     * @param [in,out]  programGUI  If non-null, the program graphical user interface.
     */
		void set_programGUI(ProgramGUI * programGUI);

		/** @brief For debugging purposes. */
		bool m_pbJumpPredatorsToBird;

	private:
    /**
     * @fn  void PhysicsEngine::launch_predators();
     *
     * @brief Executes the predators operation.
     */
		void launch_predators();

    /**
     * @fn  void PhysicsEngine::dismiss_predators();
     *
     * @brief Dismiss predators.
     */
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