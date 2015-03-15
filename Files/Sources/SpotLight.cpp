#include <SpotLight.h>
#include <ProgramGUI.h>

using namespace SweepingBirds;

SpotLight::SpotLight()
	:m_fAngle(2.13f),
	m_fFallOffAngle(3.14f)
{
	m_bIsUsed = true;
}

SpotLight::~SpotLight()
{
}

void SpotLight::set_angle(float angle)
{
	m_fAngle = angle; 
	if (m_fAngle > m_fFallOffAngle)
	{
		m_fFallOffAngle = m_fAngle;
	}
}

void SpotLight::set_falloff_angle(float angle) 
{
	m_fFallOffAngle = angle; 
	if (m_fAngle > m_fFallOffAngle)
	{
		m_fFallOffAngle = m_fAngle;
	}
}

void SpotLight::check_angles()
{
	if (m_fAngle > m_fFallOffAngle)
	{
		m_fFallOffAngle = m_fAngle;
	}
}


GUIInfos * SpotLight::generate_slider_angle_infos(std::string name)
{
	auto infos = new GUIInfos(name, 0.0f, 3.14f, 0.01f);
	infos->var.push_back(std::make_pair(name, &m_fAngle));
	return infos;
}

GUIInfos * SpotLight::generate_slider_fall_off_infos(std::string name)
{
	auto infos = new GUIInfos(name, 0.0f, 3.14f, 0.01f);
	infos->var.push_back(std::make_pair(name, &m_fFallOffAngle));
	return infos;
}
