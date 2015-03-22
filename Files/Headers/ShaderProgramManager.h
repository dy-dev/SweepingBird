#pragma once
#include <map>
#include <string>
#include <ShaderProgram.h>

namespace SweepingBirds
{
	class ShaderProgramManager
	{
	public:
		ShaderProgramManager();
		~ShaderProgramManager();

		bool create_main_shader_program();
		bool create_bird_shader_program();
		bool create_predator_shader_program();
		bool create_skybox_shader_program();
		bool create_ground_shader_program();
		bool create_lighting_shader_program();
		bool create_gamma_program();
		
		ShaderProgram* get_shader(ShaderProgramType type);

	private:
		std::map < ShaderProgramType, ShaderProgram* > m_mAvailableShaders;
	};

}