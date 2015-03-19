#include "glm/glm.hpp"
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4, glm::ivec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/gtc/type_ptr.hpp" // glm::value_ptr

#include <ShaderProgramManager.h>
#include <ShaderProgram.h>

using namespace SweepingBirds;


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
	main_program->add_var("isGround");
	main_program->set_var_value("isGround", (int)false);
	main_program->add_var("MaxMountainHeight");
	main_program->set_var_value("MaxMountainHeight", 300.0f);
	main_program->add_var("MountainFrequence");
	main_program->set_var_value("MountainFrequence", 250.0f);
	main_program->add_var("ColorControl");
	main_program->set_var_value("ColorControl", 100.0f);

	main_program->add_var("SquareSideLength");
	main_program->set_var_value("SquareSideLength", 10);

	main_program->add_var("ObjectId");
	main_program->set_var_value("ObjectId", (int)0);
	
	main_program->add_var("isBird");
	main_program->set_var_value("isBird", (int)false);

	main_program->add_var("isPredator");
	main_program->set_var_value("isPredator", (int)false);
	
	main_program->add_var("isSkyBox");
	main_program->set_var_value("isSkyBox", (int)false);

	main_program->add_var("isBat");

	main_program->add_var("BirdTranslation");
	main_program->set_var_value("BirdTranslation", glm::value_ptr(glm::vec3(0)));

	main_program->add_var("SizeFactor");
	main_program->set_var_value("SizeFactor", 1.0f);
	main_program->add_var("InstanceNumber");
	main_program->set_var_value("InstanceNumber",1);
	main_program->add_var("Translation");
	main_program->set_var_value("Translation", glm::value_ptr(glm::vec3(1)));

	main_program->add_var("MVP");
	
	main_program->add_var("InstanceNumber");
	main_program->set_var_value("InstanceNumber", 1);

	main_program->add_var("Diffuse");
	main_program->set_var_value("Diffuse", 0);

	main_program->add_var("Specular");
	main_program->set_var_value("Specular", 1);
	main_program->add_var("Ambiant");
	main_program->set_var_value("Ambiant", 2);
	main_program->add_var("Opacity");
	main_program->set_var_value("Opacity", 3);
	main_program->add_var("Shininess");
	main_program->set_var_value("Shininess", 4);

	main_program->add_var("Light3Position");
	main_program->set_var_value("Light3Position", glm::value_ptr(glm::vec3(0)));
	main_program->add_var("Light3Direction");
	main_program->set_var_value("Light3Direction", glm::value_ptr(glm::vec3(0)));
	main_program->add_var("Light3Color");
	main_program->set_var_value("Light3Color", glm::value_ptr(glm::vec3(0)));
	main_program->add_var("Light3SpecularPower");
	main_program->set_var_value("Light3SpecularPower", 40);
	main_program->add_var("Light3Use");
	main_program->set_var_value("Light3Use", (int)true);
	
	main_program->add_var("CamPos");

	main_program->add_var("Time");
	main_program->add_var("isCube");
	main_program->add_var("isLight1Marker");
	main_program->set_var_value("isLight1Marker", (int)false);
	main_program->add_var("isLight2Marker");
	main_program->set_var_value("isLight2Marker", (int)false);
	main_program->add_var("isSpotLightMarker");
	main_program->set_var_value("isSpotLightMarker", (int)false);
	main_program->add_var("Rotate");
	main_program->set_var_value("Rotate", (int)false);

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
	lighting_shader_Program->add_var("worldToLightScreen");
	lighting_shader_Program->add_var("LightPos");
	lighting_shader_Program->add_var("LightColor");
	lighting_shader_Program->add_var("LightIntensity");
	
	lighting_shader_Program->add_var("Light3Direction");
	lighting_shader_Program->set_var_value("Light3Direction", glm::value_ptr(glm::vec3(0.5)));
	lighting_shader_Program->add_var("Light3Color");
	lighting_shader_Program->set_var_value("Light3Color", glm::value_ptr(glm::vec3(1)));
	lighting_shader_Program->add_var("Light3SpecularPower");
	lighting_shader_Program->set_var_value("Light3SpecularPower", 40);
	lighting_shader_Program->set_var_value("Light3Use", (int)false);
	
	lighting_shader_Program->add_var("SpotLightPosition");
	lighting_shader_Program->set_var_value("SpotLightPosition", glm::value_ptr(glm::vec3(0)));
	lighting_shader_Program->add_var("SpotLightDirection");
	lighting_shader_Program->set_var_value("SpotLightDirection", glm::value_ptr(glm::vec3(0)));
	lighting_shader_Program->add_var("SpotLightColor");
	lighting_shader_Program->set_var_value("SpotLightColor", glm::value_ptr(glm::vec3(0)));
	lighting_shader_Program->add_var("SpotLightUse");
	lighting_shader_Program->set_var_value("SpotLightUse", (int)true);
	lighting_shader_Program->add_var("SpotLightSpecularPower");
	lighting_shader_Program->set_var_value("SpotLightSpecularPower", 40.0f);
	lighting_shader_Program->add_var("SpotLightIntensity");
	lighting_shader_Program->set_var_value("SpotLightIntensity", 40.0f);
	lighting_shader_Program->add_var("SpotLightAngle");
	lighting_shader_Program->set_var_value("SpotLightAngle", 1.0f);
	lighting_shader_Program->add_var("SpotLightFallOffAngle");
	lighting_shader_Program->set_var_value("SpotLightFallOffAngle", 1.0f);


	m_mAvailableShaders[LIGHT] = lighting_shader_Program;
	return true;

}
bool ShaderProgramManager::create_pass_through_shader_program()
{
	ShaderProgram* pass_through_shader_program = new ShaderProgram();

	pass_through_shader_program->add_shader(GL_VERTEX_SHADER, "Shaders/aogl.vert");
	pass_through_shader_program->add_shader(GL_FRAGMENT_SHADER, "Shaders/pass_through.frag");
	
	pass_through_shader_program->compile();

	pass_through_shader_program->add_var("InstanceNumber");
	pass_through_shader_program->add_var("MVP");
	pass_through_shader_program->add_var("MV");

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
	pass_through_shader_program->add_var("Light1Use");
	pass_through_shader_program->set_var_value("Light1Use", (int)true);
	
	pass_through_shader_program->add_var("Light2Pos");
	pass_through_shader_program->set_var_value("Light2Pos", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("Light2Color");
	pass_through_shader_program->set_var_value("Light2Color", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("Light2SpecularPower");
	pass_through_shader_program->set_var_value("Light2SpecularPower", 40);
	pass_through_shader_program->add_var("Light2Use");
	pass_through_shader_program->set_var_value("Light2Use", (int)true);
	
	pass_through_shader_program->add_var("Light3Position");
	pass_through_shader_program->set_var_value("Light3Position", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("Light3Direction");
	pass_through_shader_program->set_var_value("Light3Direction", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("Light3Color");
	pass_through_shader_program->set_var_value("Light3Color", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("Light3SpecularPower");
	pass_through_shader_program->set_var_value("Light3SpecularPower", 40);
	pass_through_shader_program->add_var("Light3Use");
	pass_through_shader_program->set_var_value("Light3Use", (int)true);
	
	pass_through_shader_program->add_var("SpotLightPosition");
	pass_through_shader_program->set_var_value("SpotLightPosition", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("SpotLightDirection");
	pass_through_shader_program->set_var_value("SpotLightDirection", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("SpotLightColor");
	pass_through_shader_program->set_var_value("SpotLightColor", glm::value_ptr(glm::vec3(0)));
	pass_through_shader_program->add_var("SpotLightUse");
	pass_through_shader_program->set_var_value("SpotLightUse", (int)true);
	pass_through_shader_program->add_var("SpotLightSpecularPower");
	pass_through_shader_program->set_var_value("SpotLightSpecularPower", 40.0f);
	pass_through_shader_program->add_var("SpotLightIntensity");
	pass_through_shader_program->set_var_value("SpotLightIntensity", 40.0f);
	pass_through_shader_program->add_var("SpotLightAngle");
	pass_through_shader_program->set_var_value("SpotLightAngle", 1.0f);
	pass_through_shader_program->add_var("SpotLightFallOffAngle");
	pass_through_shader_program->set_var_value("SpotLightFallOffAngle", 1.0f);
	
	pass_through_shader_program->add_var("CamPos");
	
	pass_through_shader_program->add_var("Time");
	pass_through_shader_program->add_var("isCube");
	pass_through_shader_program->add_var("isLightMarker");	
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

bool ShaderProgramManager::create_gamma_program()
{
	ShaderProgram* gamma_shader_program = new ShaderProgram();

	gamma_shader_program->add_shader(GL_VERTEX_SHADER, "Shaders/blit.vert");
	gamma_shader_program->add_shader(GL_FRAGMENT_SHADER, "Shaders/gamma.frag");

	gamma_shader_program->compile();

	gamma_shader_program->add_var("Gamma");

	m_mAvailableShaders[GAMMA] = gamma_shader_program;
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
