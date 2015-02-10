#include <Light.h>
#include <ProgramGUI.h>

Light::Light()
	:m_fSpecularPowerValue(30.0f)
{
}


Light::~Light()
{
}

GUIInfos * Light::generate_slider_position_infos(std::string name)
{
	auto infos = new GUIInfos(name, -10.0f, 10.0f, 0.1f);
	infos->var.push_back(std::make_pair(name + " X", &(m_v3Position.x)));
	infos->var.push_back(std::make_pair(name + " Y", &(m_v3Position.y)));
	infos->var.push_back(std::make_pair(name + " Z", &(m_v3Position.z)));
	return infos;
}

GUIInfos * Light::generate_slider_color_infos(std::string name)
{
	auto infos = new GUIInfos(name, 0.0, 1.0f, 0.01f);
	infos->var.push_back(std::make_pair(name + " red", &(m_v3Color.r)));
	infos->var.push_back(std::make_pair(name + " green", &(m_v3Color.g)));
	infos->var.push_back(std::make_pair(name + " blue", &(m_v3Color.b)));
	return infos;
}

GUIInfos * Light::generate_slider_specular_power_infos(std::string name)
{
	auto infos = new GUIInfos(name, 0.0f, 100.0f, 0.5f);
	infos->var.push_back(std::make_pair(name, &m_fSpecularPowerValue));
	return infos;
}