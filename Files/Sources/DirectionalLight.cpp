#include <DirectionalLight.h>
#include <ProgramGUI.h>

DirectionalLight::DirectionalLight()
{
	m_bIsUsed = true;
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
