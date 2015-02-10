#pragma once
#include <string>
#include "glm/glm.hpp"
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4, glm::ivec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/gtc/type_ptr.hpp" // glm::value_ptr

#include <Light.h>

class GUIInfos;

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

	void set_direction(glm::vec3 direction) { m_v3Direction = direction; };
	glm::vec3 get_direction(){ return m_v3Direction; };

	float * get_x_dir() { return &(m_v3Direction.x); }
	float * get_y_dir() { return &(m_v3Direction.y); }
	float * get_z_dir() { return &(m_v3Direction.z); }

	void set_color(glm::vec3 Color){ m_v3Color = Color; };;
	glm::vec3 get_color(){ return m_v3Color; };

	void set_specular_power(float value){ m_fSpecularPowerValue = value; }
	float* get_specular_power() { return &m_fSpecularPowerValue; }

	GUIInfos * generate_slider_direction_infos(std::string name);
	GUIInfos * generate_slider_color_infos(std::string name);
	GUIInfos * generate_slider_specular_power_infos(std::string name);
private:
	glm::vec3 m_v3Direction;
	glm::vec3 m_v3Color;

	float m_fSpecularPowerValue;
};

