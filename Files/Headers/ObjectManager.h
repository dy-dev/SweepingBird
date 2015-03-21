#pragma once
#include <Textured3DObject.h>


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
		ObjectManager(TextureManager * texMgr, ProgramGUI * gui);
		~ObjectManager();

		bool create_scene_assets();
		bool bind_object(Textured3DObject* object, int nb_instances);

		const std::map<std::string, std::pair<Textured3DObject*, int*> >& get_objects() { return m_mObjectManaged; };
		std::pair<Textured3DObject *, int *>& get_object(std::string name);
		
		const TextureManager* get_TexManager() const{ return m_pTexMgr; }
		const ProgramGUI* get_Gui() const{ return m_pGUI; }

		GUIInfos * generate_slider_nb_instances_infos(std::string name, int max);
		GUIInfos * generate_slider(std::string name, float min, float max, float step, float * value_to_change);
		void add_gui_controller(std::string param1, GUIInfos * param2);

	private:
		std::map<std::string , std::pair<Textured3DObject*, int*> > m_mObjectManaged;
		Bird3D * m_pBird3D;
		Predators3D * m_pPredators3D;
		SkyBoxSweepingBird* m_pSkyBox;
		Ground3D* m_pGround3D;

		TextureManager * m_pTexMgr;
		ProgramGUI * m_pGUI;
	};

}