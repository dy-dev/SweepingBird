#pragma once
#include <map>
#include <string>
#include <ShaderProgram.h>

namespace SweepingBirds
{
	class ShaderProgramManager
	{
	public:
    /**
     * @fn  ShaderProgramManager::ShaderProgramManager();
     *
     * @brief Default constructor.
     */
		ShaderProgramManager();
		~ShaderProgramManager();

    /**
     * @fn  bool ShaderProgramManager::create_main_shader_program();
     *
     * @brief Creates main shader program.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool create_main_shader_program();

    /**
     * @fn  bool ShaderProgramManager::create_bird_shader_program();
     *
     * @brief Creates bird shader program.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool create_bird_shader_program();

    /**
     * @fn  bool ShaderProgramManager::create_predator_shader_program();
     *
     * @brief Creates predator shader program.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool create_predator_shader_program();

    /**
     * @fn  bool ShaderProgramManager::create_skybox_shader_program();
     *
     * @brief Creates skybox shader program.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool create_skybox_shader_program();

    /**
     * @fn  bool ShaderProgramManager::create_ground_shader_program();
     *
     * @brief Creates ground shader program.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool create_ground_shader_program();

    /**
     * @fn  bool ShaderProgramManager::create_lighting_shader_program();
     *
     * @brief Creates lighting shader program.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool create_lighting_shader_program();

    /**
     * @fn  bool ShaderProgramManager::create_gamma_program();
     *
     * @brief Creates gamma program.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool create_gamma_program();

    /**
     * @fn  ShaderProgram* ShaderProgramManager::get_shader(ShaderProgramType type);
     *
     * @brief Gets a shader.
     *
     * @param type  The type.
     *
     * @return  null if it fails, else the shader.
     */
		ShaderProgram* get_shader(ShaderProgramType type);

	private:
		std::map < ShaderProgramType, ShaderProgram* > m_mAvailableShaders;
	};

}