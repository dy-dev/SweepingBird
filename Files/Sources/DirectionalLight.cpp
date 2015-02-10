#include <DirectionalLight.h>
#include <ProgramGUI.h>

DirectionalLight::DirectionalLight()
{
}


DirectionalLight::~DirectionalLight()
{
}


GUIInfos * DirectionalLight::generate_slider_direction_infos(std::string name)
{
	auto infos = new GUIInfos(name, -1.0f, 1.0f, 0.1f);
	infos->var.push_back(std::make_pair(name + " X", &(m_v3Direction.x)));
	infos->var.push_back(std::make_pair(name + " Y", &(m_v3Direction.y)));
	infos->var.push_back(std::make_pair(name + " Z", &(m_v3Direction.z)));
	return infos;
}

GUIInfos * DirectionalLight::generate_slider_color_infos(std::string name)
{
	auto infos = new GUIInfos(name, 0.0, 1.0f, 0.01f);
	infos->var.push_back(std::make_pair(name + " red", &(m_v3Color.r)));
	infos->var.push_back(std::make_pair(name + " green", &(m_v3Color.g)));
	infos->var.push_back(std::make_pair(name + " blue", &(m_v3Color.b)));
	return infos;
}

GUIInfos * DirectionalLight::generate_slider_specular_power_infos(std::string name)
{
	auto infos = new GUIInfos(name, 0.0f, 100.0f, 0.5f);
	infos->var.push_back(std::make_pair(name, &m_fSpecularPowerValue));
	return infos;
}
