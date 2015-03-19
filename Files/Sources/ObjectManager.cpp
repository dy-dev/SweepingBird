#include <windows.h>
#include <map>

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <Camera.h>
#include <ObjectManager.h>
#include <ProgramGUI.h>
#include <assimp/DefaultLogger.hpp>

using namespace SweepingBirds;

ObjectManager::ObjectManager()
{
}

ObjectManager::ObjectManager(int nb_objects_to_create)
	:m_iNbObjectManaged(nb_objects_to_create)
{
}

ObjectManager::~ObjectManager()
{
	for each (auto object in m_vObjectManaged)
	{
		delete object.first;
		delete object.second;
	}
}

bool ObjectManager::bind_object(Textured3DObject* object, int nb_instances, int index)
{
	if (index < m_iNbObjectManaged)
	{
		object->bind_meshes();

		m_vObjectManaged.push_back(std::make_pair(object, new int(nb_instances)));
		return true;
	}
	else
	{
		return false;
	}
}

std::pair<Textured3DObject *, int *>& ObjectManager::get_object(int index)
{
	if (index < m_vObjectManaged.size())
	{
		return m_vObjectManaged.at(index);
	}

	return *(new std::pair<Textured3DObject *, int *>());
}

GUIInfos * ObjectManager::generate_slider_nb_instances_infos(int index, int max)
{
	if (index < m_vObjectManaged.size())
	{
		auto obj = m_vObjectManaged.at(index);

		auto infos = new GUIInfos("NB instances", 0.0f, (float)max, 1.0f);
		infos->var.push_back(std::make_pair("NB instances", (float*)obj.second));

		return infos;
	}
	return nullptr;
}

GUIInfos * ObjectManager::generate_slider_cube_size(int index)
{
	if (index < m_vObjectManaged.size())
	{
		auto obj = m_vObjectManaged.at(index);

		auto infos = new GUIInfos("Cube Size", 0.10f, 5.0f, 0.01f);
		infos->var.push_back(std::make_pair("Cube Size", obj.first->get_size()));

		return infos;
	}
	return nullptr;
}

GUIInfos * ObjectManager::generate_slider_cube_radius_spacing(int index)
{
	if (index < m_vObjectManaged.size())
	{
		auto obj = m_vObjectManaged.at(index);

		auto infos = new GUIInfos("Radius Spacing", 0.1f, 2.0f, 0.10f);
		infos->var.push_back(std::make_pair("Radius Spacing", obj.first->get_radius_spacing()));

		return infos;
	}
	return nullptr;
}

GUIInfos * ObjectManager::generate_slider_cube_range(int index)
{
	if (index < m_vObjectManaged.size())
	{
		auto obj = m_vObjectManaged.at(index);

		auto infos = new GUIInfos("Range", 0.0f, 5.0, 0.10f);
		infos->var.push_back(std::make_pair(infos->name, obj.first->get_range()));

		return infos;
	}
	return nullptr;
}

GUIInfos * ObjectManager::generate_slider_cube_speed(int index)
{
	if (index < m_vObjectManaged.size())
	{
		auto obj = m_vObjectManaged.at(index);

		auto infos = new GUIInfos("Speed", 0.0f, 100.0, 0.10f);
		infos->var.push_back(std::make_pair(infos->name, obj.first->get_speed()));

		return infos;
	}
	return nullptr;
}

GUIInfos * ObjectManager::generate_check_cube_rotation(int index)
{
	if (index < m_vObjectManaged.size())
	{
		auto obj = m_vObjectManaged.at(index);

		auto infos = new GUIInfos("Rotate Cubes");
		infos->check_adress = obj.first->is_rotating();

		return infos;
	}
	return nullptr;
}

GUIInfos * ObjectManager::generate_slider(std::string name, float min, float max, float step, float * value_to_change)
{
	auto infos = new GUIInfos(name, min, max, step);
	infos->var.push_back(std::make_pair(infos->name, value_to_change));

	return infos;
}



