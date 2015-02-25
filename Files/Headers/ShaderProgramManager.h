#pragma once
#include <map>
#include <string>
#include <ShaderProgram.h>

class ShaderProgramManager
{
public:
	ShaderProgramManager();
	~ShaderProgramManager();

	bool create_main_shader_program();
	bool create_blit_shader_program();
	bool create_lighting_shader_program();
	bool create_blit_point_light_shader_program();
	bool create_pass_through_shader_program();
	bool create_gamma_program();
	bool create_sobel_program();

	ShaderProgram* get_shader(ShaderProgramType type);

private:
	std::map < ShaderProgramType, ShaderProgram* > m_mAvailableShaders;
};

