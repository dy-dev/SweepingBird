#pragma once
#include <DirectionalLight.h>

namespace SweepingBirds
{
struct GUIInfos;

	class SpotLight : public DirectionalLight
	{
	public:
    /**
     * @fn  SpotLight::SpotLight();
     *
     * @brief Default constructor.
     */
		SpotLight();

    /**
     * @fn  SpotLight::~SpotLight();
     *
     * @brief Destructor.
     */
		~SpotLight();

    /**
     * @fn  void SpotLight::set_angle(float angle);
     *
     * @brief Sets an angle.
     *
     * @param angle The angle.
     */
		void set_angle(float angle);

    /**
     * @fn  float SpotLight::get_angle()
     *
     * @brief Gets the angle.
     *
     * @return  The angle.
     */
		float get_angle() { return m_fAngle; }

    /**
     * @fn  void SpotLight::set_falloff_angle(float angle);
     *
     * @brief Sets falloff angle.
     *
     * @param angle The angle.
     */
		void set_falloff_angle(float angle);

    /**
     * @fn  float SpotLight::get_falloff_angle()
     *
     * @brief Gets falloff angle.
     *
     * @return  The falloff angle.
     */
		float get_falloff_angle() { return m_fFallOffAngle; }

    /**
     * @fn  void SpotLight::check_angles();
     *
     * @brief Check angles.
     */
		void check_angles();

    /**
     * @fn  GUIInfos * SpotLight::generate_slider_angle_infos(std::string name);
     *
     * @brief Generates a slider angle infos.
     *
     * @param name  The name.
     *
     * @return  null if it fails, else the slider angle infos.
     */
		GUIInfos * generate_slider_angle_infos(std::string name);

    /**
     * @fn  GUIInfos * SpotLight::generate_slider_fall_off_infos(std::string name);
     *
     * @brief Generates a slider fall off infos.
     *
     * @param name  The name.
     *
     * @return  null if it fails, else the slider fall off infos.
     */
		GUIInfos * generate_slider_fall_off_infos(std::string name);

	protected:
		float m_fAngle;
		float m_fFallOffAngle;

	};

}