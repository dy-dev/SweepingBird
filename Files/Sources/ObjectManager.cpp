#include <map>

#include "glew/glew.h"
#include "GLFW/glfw3.h"

#include <Camera.h>
#include <ObjectManager.h>
#include <ProgramGUI.h>


ObjectManager::ObjectManager()
{
}

ObjectManager::ObjectManager(int nb_objects_to_create)
	:m_iNbObjectManaged(nb_objects_to_create)
{
	m_pVao = new GLuint[nb_objects_to_create];
	glGenVertexArrays(nb_objects_to_create, m_pVao);
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
		//Bind the VAO
		glBindVertexArray(m_pVao[index]);
		object->set_associated_vao(m_pVao[index]);

		GLuint vbo[4];
		glGenBuffers(4, vbo);
		// Bind indices and upload data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->get_trianglesList().size() * sizeof(int), object->get_trianglesList().data(), GL_STATIC_DRAW);

		// Bind vertices and upload data
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, object->get_dimension(), GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * object->get_dimension(), (void*)0);
		glBufferData(GL_ARRAY_BUFFER, object->get_vertices().size()* sizeof(float), object->get_vertices().data(), GL_STATIC_DRAW);

		if (object->get_dimension() == 3)
		{
			// Bind normals and upload data
			glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, (void*)0);
			glBufferData(GL_ARRAY_BUFFER, object->get_normals().size()* sizeof(float), object->get_normals().data(), GL_STATIC_DRAW);

			// Bind uv coords and upload data
			glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, (void*)0);
			glBufferData(GL_ARRAY_BUFFER, object->get_uvs().size()* sizeof(float), object->get_uvs().data(), GL_STATIC_DRAW);
		}
		// Unbind everything
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_vObjectManaged.push_back(std::make_pair(object, new float(nb_instances)));
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

		auto infos = new GUIInfos("Speed", 0.0f, 5.0, 0.10f);
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

	return std::pair<Textured3DObject *, float *>();
}
