#pragma once
#include <Textured3DObject.h>
#include <functional>

namespace SweepingBirds
{
	class Camera;
	class Bird3D;
	class Predators3D;
	class Ground3D;
	class SkyBoxSweepingBird;
	struct GUIInfos;

	class ObjectManager
	{
	public:
		ObjectManager();
		ObjectManager(TextureManager * texMgr, ProgramGUI * gui, Camera * cam);
		~ObjectManager();

		bool bind_object(Textured3DObject* object, int nb_instances);

		const std::multimap<std::string, std::pair<Textured3DObject*, float*> >& get_objects() { return m_mObjectManaged; };
		std::pair<Textured3DObject *, float *>& get_object(std::string name);
		
		const TextureManager* get_TexManager() const{ return m_pTexMgr; }
		const ProgramGUI* get_Gui() const{ return m_pGUI; }

		GUIInfos * generate_slider_nb_instances_infos(std::string name, int max);
		GUIInfos * generate_slider(std::string name, float min, float max, float step, float * value_to_change);
		GUIInfos * generate_button(std::string name, std::function<void(void*)> function_to_call, void *ob);
			
		void add_gui_controller(std::string param1, GUIInfos * param2);

		const Camera* get_Camera() const {return m_pCamera;}
		void stick_cam(Textured3DObject* obj);
	private:
		//need a float as nbInstance not an int because of GUI limitation
		std::multimap<std::string , std::pair<Textured3DObject*, float*> > m_mObjectManaged;

		TextureManager * m_pTexMgr;
		ProgramGUI * m_pGUI;
		Camera * m_pCamera;
	};

}