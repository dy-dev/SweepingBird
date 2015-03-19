#pragma once
#include <Textured3DObject.h>


namespace SweepingBirds
{
	class Camera;
	struct GUIInfos;

	class ObjectManager
	{
	public:
		ObjectManager();
		ObjectManager(int nb_objects_to_create);
		~ObjectManager();

		void createAILogger();

		bool bind_object(Textured3DObject* object, int nb_instances, int index);
		const std::vector<std::pair<Textured3DObject*, int*> >& get_objects() { return m_vObjectManaged; };
		std::pair<Textured3DObject *, int *>& get_object(int index);
		GUIInfos * generate_slider_nb_instances_infos(int index, int max);
		GUIInfos * generate_slider_cube_size(int index);
		GUIInfos * generate_slider_cube_radius_spacing(int index);
		GUIInfos * generate_slider_cube_speed(int index);
		GUIInfos * generate_slider_cube_range(int index);
		GUIInfos * generate_check_cube_rotation(int index);
		GUIInfos * generate_slider(std::string name, float min, float max, float step, float * value_to_change);
	private:
		int m_iNbObjectManaged;
		std::vector<std::pair<Textured3DObject*, int*> > m_vObjectManaged;

	};

}