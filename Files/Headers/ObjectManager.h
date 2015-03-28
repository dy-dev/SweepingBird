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
    /**
     * @fn  ObjectManager::ObjectManager();
     *
     * @brief Default constructor.
     */
		ObjectManager();

    /**
     * @fn  ObjectManager::ObjectManager(TextureManager * texMgr, ProgramGUI * gui, Camera * cam);
     *
     * @brief Constructor.
     *
     * @param [in,out]  texMgr  If non-null, manager for tex.
     * @param [in,out]  gui     If non-null, the graphical user interface.
     * @param [in,out]  cam     If non-null, the camera.
     */
		ObjectManager(TextureManager * texMgr, ProgramGUI * gui, Camera * cam);
		~ObjectManager();

    /**
     * @fn  bool ObjectManager::bind_object(Textured3DObject* object, int nb_instances);
     *
     * @brief Bind object.
     *
     * @param [in,out]  object  If non-null, the object.
     * @param nb_instances      The nb instances.
     *
     * @return  true if it succeeds, false if it fails.
     */
		bool bind_object(Textured3DObject* object, int nb_instances);

    /**
     * @fn  const std::multimap<std::string, std::pair<Textured3DObject*, float*> >& ObjectManager::get_objects()
     *
     * @brief Gets the objects.
     *
     * @return  null if it fails, else the objects.
     */
		const std::multimap<std::string, std::pair<Textured3DObject*, float*> >& get_objects() { return m_mObjectManaged; };

    /**
     * @fn  std::pair<Textured3DObject *, float *>& ObjectManager::get_object(std::string name);
     *
     * @brief Gets an object.
     *
     * @param name  The name.
     *
     * @return  null if it fails, else the object.
     */
		std::pair<Textured3DObject *, float *>& get_object(std::string name);

    /**
     * @fn  const TextureManager* ObjectManager::get_TexManager() const
     *
     * @brief Gets tex manager.
     *
     * @return  null if it fails, else the tex manager.
     */
		const TextureManager* get_TexManager() const{ return m_pTexMgr; }

    /**
     * @fn  const ProgramGUI* ObjectManager::get_Gui() const
     *
     * @brief Gets the graphical user interface.
     *
     * @return  null if it fails, else the graphical user interface.
     */
		const ProgramGUI* get_Gui() const{ return m_pGUI; }

    /**
     * @fn  GUIInfos * ObjectManager::generate_slider_nb_instances_infos(Textured3DObject* object, int max);
     *
     * @brief Generates a slider nb instances infos.
     *
     * @param [in,out]  object  If non-null, the object.
     * @param max               The maximum.
     *
     * @return  null if it fails, else the slider nb instances infos.
     */
		GUIInfos * generate_slider_nb_instances_infos(Textured3DObject* object, int max);

    /**
     * @fn  GUIInfos * ObjectManager::generate_slider(std::string name, float min, float max, float step, float * value_to_change);
     *
     * @brief Generates a slider.
     *
     * @param name                      The name.
     * @param min                       The minimum.
     * @param max                       The maximum.
     * @param step                      Amount to increment by.
     * @param [in,out]  value_to_change If non-null, the value to change.
     *
     * @return  null if it fails, else the slider.
     */
		GUIInfos * generate_slider(std::string name, float min, float max, float step, float * value_to_change);

    /**
     * @fn  GUIInfos * ObjectManager::generate_button(std::string name, std::function<void(void*, bool)> function_to_call, void *ob);
     *
     * @brief Generates a button.
     *
     * @param name                        The name.
     * @param [in,out]  function_to_call  If non-null, the function to call.
     * @param [in,out]  ob                If non-null, the ob.
     *
     * @return  null if it fails, else the button.
     */
		GUIInfos * generate_button(std::string name, std::function<void(void*, bool)> function_to_call, void *ob);

    /**
     * @fn  GUIInfos * ObjectManager::generate_checkbox(std::string name, bool * is_used);
     *
     * @brief Generates a checkbox.
     *
     * @param name              The name.
     * @param [in,out]  is_used If non-null, the is used.
     *
     * @return  null if it fails, else the checkbox.
     */
		GUIInfos * generate_checkbox(std::string name, bool * is_used);

    /**
     * @fn  void ObjectManager::add_gui_controller(std::string param1, GUIInfos * param2);
     *
     * @brief Adds a graphical user interface controller to 'param2'.
     *
     * @param param1            The first parameter.
     * @param [in,out]  param2  If non-null, the second parameter.
     */
		void add_gui_controller(std::string param1, GUIInfos * param2);

    /**
     * @fn  const Camera* ObjectManager::get_Camera() const
     *
     * @brief Gets the camera.
     *
     * @return  null if it fails, else the camera.
     */
		const Camera* get_Camera() const {return m_pCamera;}

    /**
     * @fn  void ObjectManager::jump_cam(Textured3DObject* obj);
     *
     * @brief Jump camera.
     *
     * @param [in,out]  obj If non-null, the object.
     */
		void jump_cam(Textured3DObject* obj);	

	private:
		//need a float as nbInstance not an int because of GUI limitation
		std::multimap<std::string , std::pair<Textured3DObject*, float*> > m_mObjectManaged;

		TextureManager * m_pTexMgr;
		ProgramGUI * m_pGUI;
		Camera * m_pCamera;
	};

}