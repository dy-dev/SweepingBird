#include <windows.h>
#include <map>

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <Camera.h>
#include <ObjectManager.h>
#include <ProgramGUI.h>
#include <assimp/DefaultLogger.hpp>


ObjectManager::ObjectManager()
{
	createAILogger();
}

ObjectManager::ObjectManager(int nb_objects_to_create)
	:m_iNbObjectManaged(nb_objects_to_create)
{
	createAILogger();
}

ObjectManager::~ObjectManager()
{
	for each (auto object in m_vObjectManaged)
	{
		delete object.first;
		delete object.second;
	}
}

void ObjectManager::createAILogger()
{
	//Assimp::Logger::LogSeverity severity = Assimp::Logger::NORMAL;
	Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;

	// Create a loggr instance for Console Output
	Assimp::DefaultLogger::create("", severity, aiDefaultLogStream_STDOUT);

	// Create a logger instance for File Output (found in project folder or near .exe)
	Assimp::DefaultLogger::create("assimp_log.txt", severity, aiDefaultLogStream_FILE);

	// Now I am ready for logging my stuff
	Assimp::DefaultLogger::get()->info("this is my info-call");
}

bool ObjectManager::bind_object(Textured3DObject* object, int nb_instances, int index)
{
	if (index < m_iNbObjectManaged)
	{
		object->bind_meshes();
		
		m_vObjectManaged.push_back(std::make_pair(object, new float((float)nb_instances)));
		return true;
	}
	else
	{
		return false;
	}
}

GUIInfos * ObjectManager::generate_slider_nb_instances_infos(int index, float max)
{
	if (index < m_vObjectManaged.size())
	{
		auto obj = m_vObjectManaged.at(index);

		auto infos = new GUIInfos("NB Cube instances", 0.0f, max, 1.0f);
		infos->var.push_back(std::make_pair("NB Cube instances", obj.second));

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

std::pair<Textured3DObject *, float *>& ObjectManager::get_object(int index)
{
	if (index < m_vObjectManaged.size())
	{
		return m_vObjectManaged.at(index);
	}

	return *(new std::pair<Textured3DObject *, float *>());
}


