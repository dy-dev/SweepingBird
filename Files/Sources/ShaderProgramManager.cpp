#include "glm/glm.hpp"
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4, glm::ivec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/gtc/type_ptr.hpp" // glm::value_ptr

#include <ShaderProgramManager.h>
#include <ShaderProgram.h>

ShaderProgramManager::ShaderProgramManager()
{
}


ShaderProgramManager::~ShaderProgramManager()
{
	for each (auto shader in m_mAvailableShaders)
	{
		delete shader.second;
	}
}

bool ShaderProgramManager::create_main_shader_program()
{
	ShaderProgram *main_program = new ShaderProgram();
	main_program->add_shader(GL_VERTEX_SHADER, "Shaders/aogl.vert");
	main_program->add_shader(GL_FRAGMENT_SHADER, "Shaders/aogl.frag");

	main_program->compile();

	main_program->add_var("InstanceNumber");
	main_program->add_var("MVP");
	
	main_program->add_var("InstanceNumber");
	main_program->set_var_value("InstanceNumber", 1);

	main_program->add_var("Diffuse");
	main_program->set_var_value("Diffuse", 0);

	main_program->add_var("Specular");
	main_program->set_var_value("Specular", 1);

	
	main_program->add_var("Light1Pos");
	main_program->set_var_value("Light1Pos", glm::value_ptr(glm::vec3(0)));
	main_program->add_var("Light1Color");
	main_program->set_var_value("Light1Color", glm::value_ptr(glm::vec3(0)));
	main_program->add_var("Light1SpecularPower");
	main_program->set_var_value("Light1SpecularPower", 20);

	main_program->add_var("Light2Pos");
	main_program->set_var_value("Light2Pos", glm::value_ptr(glm::vec3(0)));
	main_program->add_var("Light2Color");
	main_program->set_var_value("Light2Color", glm::value_ptr(glm::vec3(0)));
	main_program->add_var("Light2SpecularPower");
	main_program->set_var_value("Light2SpecularPower", 40);

	main_program->add_var("Light3Direction");
	main_program->set_var_value("Light3Direction", glm::value_ptr(glm::vec3(0)));
	main_program->add_var("Light3Color");
	main_program->set_var_value("Light3Color", glm::value_ptr(glm::vec3(0)));
	main_program->add_var("Light3SpecularPower");
	main_program->set_var_value("Light3SpecularPower", 40);

	main_program->add_var("CamPos");

	main_program->add_var("Time");
	main_program->add_var("isCube");
	main_program->add_var("Rotate");
	main_program->set_var_value("Rotate", (int)false);

	main_program->add_var("SizeFactor");
	main_program->set_var_value("SizeFactor", 1.0f);

	main_program->add_var("RadiusSpacing");
	main_program->set_var_value("RadiusSpacing", 1.0f);

	main_program->add_var("SpeedFactor");
	main_program->set_var_value("SpeedFactor", 1.0f);

	main_program->add_var("RangeFactor");
	main_program->set_var_value("RangeFactor", 1.0f);


	m_mAvailableShaders[MAIN] = main_program;

	return true;
}

bool ShaderProgramManager::create_blit_shader_program()
{
	ShaderProgram* blit_shader_Program = new ShaderProgram();

	blit_shader_Program->add_shader(GL_VERTEX_SHADER, "Shaders/blit.vert");
	blit_shader_Program->add_shader(GL_FRAGMENT_SHADER, "Shaders/blit.frag");

	blit_shader_Program->compile();

	blit_shader_Program->add_var("Texture");
	blit_shader_Program->set_var_value("Texture", 0);

	m_mAvailableShaders[BLIT] = blit_shader_Program;
	return true;
}

bool ShaderProgramManager::create_lighting_shader_program()
{
	ShaderProgram* lighting_shader_Program = new ShaderProgram();

	lighting_shader_Program->add_shader(GL_VERTEX_SHADER, "Shaders/lighting.vert");
	lighting_shader_Program->add_shader(GL_FRAGMENT_SHADER, "Shaders/lighting.frag");

	lighting_shader_Program->compile();
	
	lighting_shader_Program->add_var("ColorBuffer");
	lighting_shader_Program->set_var_value("ColorBuffer", 0);
	lighting_shader_Program->add_var("NormalBuffer");
	lighting_shader_Program->set_var_value("NormalBuffer", 1);
	lighting_shader_Program->add_var("DepthBuffer");
	lighting_shader_Program->set_var_value("DepthBuffer", 2);
	
	lighting_shader_Program->add_var("ShadowBuffer");
	lighting_shader_Program->set_var_value("ShadowBuffer", 3);

	

	lighting_shader_Program->add_var("CamPos");
	lighting_shader_Program->add_var("ScreenToWorld");
	lighting_shader_Program->add_var("Light1Pos");
	lighting_shader_Program->add_var("Light1Color");
	lighting_shader_Program->set_var_value("Light1SpecularPower", 40);

	lighting_shader_Program->add_var("Light3Direction");
	lighting_shader_Program->set_var_value("Light3Direction", glm::value_ptr(glm::vec3(0.5)));
	lighting_shader_Program->add_var("Light3Color");
	lighting_shader_Program->set_var_value("Light3Color", glm::value_ptr(glm::vec3(1)));
	lighting_shader_Program->add_var("Light3SpecularPower");
	lighting_shader_Program->set_var_value("Light3SpecularPower", 40);

	m_mAvailableShaders[LIGHT] = lighting_shader_Program;
	return true;

}
bool ShaderProgramManager::create_pass_through_shader_program()
{
	ShaderProgram* pass_through_shader_program = new ShaderProgram();

	pass_through_shader_program->add_shader(GL_VERTEX_SHADER, "Shaders/aogl.vert");
	pass_through_shader_program->add_shader(GL_FRAGMENT_SHADER, "Shaders/pass_through.frag");
	
	pass_through_shader_program->compile();
	
	pass_through_shader_program->add_var("ObjectToLightScreen");
	pass_through_shader_program->add_var("ObjectToLight");
	

	pass_through_shader_program->add_var("InstanceNumber");
	pass_through_shader_program->add_var("MVP");
	
	pass_through_shader_program->add_var("InstanceNumber");
	pass_through_shader_program->set_var_value("InstanceNumber", 1);
	
	pass_through_shader_program->add_var("Diffuse");
	pass_through_shader_program->set_var_value("Diffuse", 0);
	
	pass_through_shader_program->add_var("Specular");
	pass_through_shader_program->set_var_value("Specular", 1);
	
	pass_through_shader_program->add_var("Light1Pos");
	pass_through_shader_program->set_var_value("Light1Pos", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("Light1Color");
	pass_through_shader_program->set_var_value("Light1Color", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("Light1SpecularPower");
	pass_through_shader_program->set_var_value("Light1SpecularPower", 20);
	
	pass_through_shader_program->add_var("Light2Pos");
	pass_through_shader_program->set_var_value("Light2Pos", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("Light2Color");
	pass_through_shader_program->set_var_value("Light2Color", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("Light2SpecularPower");
	pass_through_shader_program->set_var_value("Light2SpecularPower", 40);
	
	pass_through_shader_program->add_var("Light3Position");
	pass_through_shader_program->set_var_value("Light3Position", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("Light3Direction");
	pass_through_shader_program->set_var_value("Light3Direction", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("Light3Color");
	pass_through_shader_program->set_var_value("Light3Color", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("Light3SpecularPower");
	pass_through_shader_program->set_var_value("Light3SpecularPower", 40);
	
	pass_through_shader_program->add_var("CamPos");
	
	pass_through_shader_program->add_var("Time");
	pass_through_shader_program->add_var("isCube");
	pass_through_shader_program->add_var("Rotate");
	pass_through_shader_program->set_var_value("Rotate", (int)false);
	
	pass_through_shader_program->add_var("SizeFactor");
	pass_through_shader_program->set_var_value("SizeFactor", 1.0f);
	
	pass_through_shader_program->add_var("RadiusSpacing");
	pass_through_shader_program->set_var_value("RadiusSpacing", 1.0f);
	
	pass_through_shader_program->add_var("SpeedFactor");
	pass_through_shader_program->set_var_value("SpeedFactor", 1.0f);
	
	pass_through_shader_program->add_var("RangeFactor");
	pass_through_shader_program->set_var_value("RangeFactor", 1.0f);

	m_mAvailableShaders[SHADOW_MAP] = pass_through_shader_program;

	return true;
}



ShaderProgram* ShaderProgramManager::get_shader(ShaderProgramType type)
{
	auto shader = m_mAvailableShaders.find(type);
	if (shader != m_mAvailableShaders.end())
	{
		return shader->second;
	}
	return nullptr;
}
