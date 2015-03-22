#include <windows.h>
#include <map>

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <Bird3D.h>
#include <Predators3D.h>
#include <Ground.h>
#include <SkyBoxSweepingBird.h>
#include <Camera.h>
#include <ObjectManager.h>
#include <ProgramGUI.h>
#include <assimp/DefaultLogger.hpp>

using namespace SweepingBirds;

ObjectManager::ObjectManager()
	:m_pBird3D(nullptr),
	m_pPredators3D(nullptr),
	m_pGround3D(nullptr),
	m_pSkyBox(nullptr),
	m_pGUI(nullptr),
	m_pTexMgr(nullptr)
{
}

ObjectManager::ObjectManager(TextureManager * texMgr, ProgramGUI * gui)
	:ObjectManager()
{
	m_pTexMgr = texMgr;
	m_pGUI = gui;
}

ObjectManager::~ObjectManager()
{
	for each (auto object in m_mObjectManaged)
	{
		delete object.second.first;
		delete object.second.second;
	}
}

bool ObjectManager::create_scene_assets()
{
	m_pBird3D = new Bird3D(this, m_pTexMgr);
	m_pPredators3D = new Predators3D(this, m_pTexMgr,50);
	m_pGround3D = new Ground3D(this, m_pTexMgr,5000);
	m_pSkyBox = new SkyBoxSweepingBird(this, m_pTexMgr);
	return true;
}

void ObjectManager::add_gui_controller(std::string name, GUIInfos * infos)
{
	m_pGUI->add_gui_element(name, infos);
}

bool ObjectManager::bind_object(Textured3DObject* object, int nb_instances)
{
	if (object != nullptr)
	{
		object->bind_meshes();

		m_mObjectManaged[object->get_name()] = std::make_pair(object, new float(nb_instances));
		return true;
	}
	else
	{
		return false;
	}
}

std::pair<Textured3DObject *, float *>& ObjectManager::get_object(std::string name)
{
	auto toRet = m_mObjectManaged.find(name);
	if (toRet != m_mObjectManaged.end())
	{
		return toRet->second;
	}

	return *(new std::pair<Textured3DObject *, float *>(nullptr,nullptr));
}

GUIInfos * ObjectManager::generate_slider_nb_instances_infos(std::string name, int max)
{
	auto toRet = m_mObjectManaged.find(name);
	if (toRet != m_mObjectManaged.end())
	{
		auto infos = new GUIInfos("NB instances", 0.0f, (float)max, 1.0f);
		infos->var.push_back(std::make_pair("NB instances", (float*)(toRet->second.second)));

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
