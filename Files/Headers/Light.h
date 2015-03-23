#pragma once
#include <string>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

namespace SweepingBirds
{
struct GUIInfos;

	class Light
	{
	public:
		Light();
		~Light();

		void set_position(glm::vec3 Position) { m_v3Position = Position; };
		glm::vec3 get_position(){ return m_v3Position; };

		float * get_x_pos() { return &(m_v3Position.x); }
		float * get_y_pos() { return &(m_v3Position.y); }
		float * get_z_pos() { return &(m_v3Position.z); }

		void set_color(glm::vec3 Color){ m_v3Color = Color; };;
		glm::vec3 get_color(){ return m_v3Color; };

		void set_specular_power(float value){ m_fSpecularPowerValue = value; }
		float get_specular_power() { return m_fSpecularPowerValue; }

		void set_is_in_use(bool inUse) { m_bIsUsed = inUse; }
		bool get_is_in_use() { return m_bIsUsed; }

		GUIInfos * generate_slider_position_infos(std::string name);
		GUIInfos * generate_slider_color_infos(std::string name);
		GUIInfos * generate_slider_specular_power_infos(std::string name);
		GUIInfos * generate_use_light_checkbox(std::string name);
		GUIInfos * generate_slider_intensity_infos(std::string name);

		virtual void set_intensity(float intensity) { m_fIntensity = intensity; }
		virtual float get_intensity() { return m_fIntensity; }

	protected:
		glm::vec3 m_v3Position;
		glm::vec3 m_v3Color;

		float m_fIntensity;
		float m_fSpecularPowerValue;
		bool m_bIsUsed;
	};

}