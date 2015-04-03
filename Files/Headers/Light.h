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
    /**
     * @fn  Light::Light();
     *
     * @brief Default constructor.
     */
		Light();
		~Light();

    /**
     * @fn  void Light::set_position(glm::vec3 Position)
     *
     * @brief Sets a position.
     *
     * @param Position  The position.
     */
		void set_position(glm::vec3 Position) { m_v3Position = Position; };

    /**
     * @fn  glm::vec3 Light::get_position()
     *
     * @brief Gets the position.
     *
     * @return  The position.
     */
		glm::vec3 get_position(){ return m_v3Position; };

    /**
     * @fn  float * Light::get_x_pos()
     *
     * @brief Get x coordinate position.
     *
     * @return  null if it fails, else the x coordinate position.
     */
		float * get_x_pos() { return &(m_v3Position.x); }

    /**
     * @fn  float * Light::get_y_pos()
     *
     * @brief Get y coordinate position.
     *
     * @return  null if it fails, else the y coordinate position.
     */
		float * get_y_pos() { return &(m_v3Position.y); }

    /**
     * @fn  float * Light::get_z_pos()
     *
     * @brief Get z coordinate position.
     *
     * @return  null if it fails, else the z coordinate position.
     */
		float * get_z_pos() { return &(m_v3Position.z); }

    /**
     * @fn  void Light::set_color(glm::vec3 Color)
     *
     * @brief Sets a color.
     *
     * @param Color The color.
     */
		void set_color(glm::vec3 Color){ m_v3Color = Color; };;

    /**
     * @fn  glm::vec3 Light::get_color()
     *
     * @brief Gets the color.
     *
     * @return  The color.
     */
		glm::vec3 get_color(){ return m_v3Color; };

    /**
     * @fn  void Light::set_specular_power(float value)
     *
     * @brief Sets specular power.
     *
     * @param value The value.
     */
		void set_specular_power(float value){ m_fSpecularPowerValue = value; }

    /**
     * @fn  float Light::get_specular_power()
     *
     * @brief Gets specular power.
     *
     * @return  The specular power.
     */
		float get_specular_power() { return m_fSpecularPowerValue; }

    /**
     * @fn  void Light::set_is_in_use(bool inUse)
     *
     * @brief Sets is in use.
     *
     * @param inUse true to in use.
     */
		void set_is_in_use(bool inUse) { m_bIsUsed = inUse; }

    /**
     * @fn  bool Light::get_is_in_use()
     *
     * @brief Gets is in use.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool get_is_in_use() { return m_bIsUsed; }

    /**
     * @fn  GUIInfos * Light::generate_slider_position_infos(std::string name);
     *
     * @brief Generates a slider position infos.
     *
     * @param name  The name.
     *
     * @return  null if it fails, else the slider position infos.
     */
		GUIInfos * generate_slider_position_infos(std::string name);

    /**
     * @fn  GUIInfos * Light::generate_slider_color_infos(std::string name);
     *
     * @brief Generates a slider color infos.
     *
     * @param name  The name.
     *
     * @return  null if it fails, else the slider color infos.
     */
		GUIInfos * generate_slider_color_infos(std::string name);

    /**
     * @fn  GUIInfos * Light::generate_slider_specular_power_infos(std::string name);
     *
     * @brief Generates a slider specular power infos.
     *
     * @param name  The name.
     *
     * @return  null if it fails, else the slider specular power infos.
     */
		GUIInfos * generate_slider_specular_power_infos(std::string name);

    /**
     * @fn  GUIInfos * Light::generate_use_light_checkbox(std::string name);
     *
     * @brief Generates an use light checkbox.
     *
     * @param name  The name.
     *
     * @return  null if it fails, else the use light checkbox.
     */
		GUIInfos * generate_use_light_checkbox(std::string name);

    /**
     * @fn  GUIInfos * Light::generate_slider_intensity_infos(std::string name);
     *
     * @brief Generates a slider intensity infos.
     *
     * @param name  The name.
     *
     * @return  null if it fails, else the slider intensity infos.
     */
		GUIInfos * generate_slider_intensity_infos(std::string name);

    /**
     * @fn  void Light::set_intensity(float intensity)
     *
     * @brief Sets an intensity.
     *
     * @param intensity The intensity.
     */
		void set_intensity(float intensity) { m_fIntensity = intensity; }

    /**
     * @fn  float Light::get_intensity()
     *
     * @brief Gets the intensity.
     *
     * @return  The intensity.
     */
	virtual	float get_intensity() { return m_fIntensity; }

	protected:
		glm::vec3 m_v3Position;
		glm::vec3 m_v3Color;

		float m_fIntensity;
		float m_fSpecularPowerValue;
		bool m_bIsUsed;
	};

}