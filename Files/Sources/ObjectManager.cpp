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
#include <PhysicsEngine.h>
#include <SceneManager.h>

using namespace SweepingBirds;

ObjectManager::ObjectManager()
	: m_pGUI(nullptr),
	m_pTexMgr(nullptr),
	m_pCamera(nullptr),
	m_pSceneManager(nullptr)
{

}

ObjectManager::ObjectManager(TextureManager * texMgr, ProgramGUI * gui, Camera * cam, SceneManager * sceneMgr)
	:ObjectManager()
{
	m_pTexMgr = texMgr;
	m_pGUI = gui;
	m_pCamera = cam;
	m_pSceneManager = sceneMgr;

	//These objects register themselve to the ObjectManager (this object)
	new Bird3D(this, texMgr);
	new Predators3D(this, texMgr, PhysicsEngine::NB_PREDATORS);
	new Ground3D(this, texMgr, 2);
	new SkyBoxSweepingBird(this, texMgr);
}

ObjectManager::~ObjectManager()
{
	for each (auto object in m_mObjectManaged)
	{
		delete object.second.first;
		delete object.second.second;
	}
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
		m_mObjectManaged.emplace(std::string(typeid(*object).name()), std::make_pair(object, new float(nb_instances)));
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

	return *(new std::pair<Textured3DObject *, float *>(nullptr, nullptr));
}

GUIInfos * ObjectManager::generate_slider_nb_instances_infos(Textured3DObject* object, int max)
{
	auto name = std::string(typeid(*object).name());
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

GUIInfos * ObjectManager::generate_button(std::string name, std::function<void(void*, bool)> function_to_call, void *obj)
{
	auto infos = new GUIInfos(name, BUTTON);
	infos->var.push_back(std::make_pair(infos->name, new float(0)));
	infos->button_action = function_to_call;
	infos->obj = obj;
	return infos;
}

GUIInfos * ObjectManager::generate_checkbox(std::string name, bool * is_used)
{
	auto infos = new GUIInfos(name, CHECKBOX);
	infos->check_adress = is_used;

	return infos;
}

void ObjectManager::jump_cam(Textured3DObject* obj)
{
	m_pCamera->jump_to_pos(obj->get_position(), obj->get_direction());
}