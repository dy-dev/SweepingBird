#pragma once
#include <DirectionalLight.h>

namespace SweepingBirds
{
struct GUIInfos;

	class SpotLight : public DirectionalLight
	{
	public:
		SpotLight();
		~SpotLight();

		void set_angle(float angle);
		float get_angle() { return m_fAngle; }

		void set_falloff_angle(float angle);
		float get_falloff_angle() { return m_fFallOffAngle; }

		void check_angles();



		GUIInfos * generate_slider_angle_infos(std::string name);
		GUIInfos * generate_slider_fall_off_infos(std::string name);

	protected:
		float m_fAngle;
		float m_fFallOffAngle;

	};

}