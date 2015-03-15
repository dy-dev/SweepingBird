#pragma once
#include <string>
#include "glm/glm.hpp"
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4, glm::ivec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/gtc/type_ptr.hpp" // glm::value_ptr

#include <Light.h>

namespace SweepingBirds
{
	struct GUIInfos;

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		~DirectionalLight();

		void set_direction(glm::vec3 direction) { m_v3Direction = direction; };
		glm::vec3 get_direction(){ return m_v3Direction; };

		float * get_x_dir() { return &(m_v3Direction.x); }
		float * get_y_dir() { return &(m_v3Direction.y); }
		float * get_z_dir() { return &(m_v3Direction.z); }

		GUIInfos * generate_slider_direction_infos(std::string name);

	protected:
		glm::vec3 m_v3Direction;
	};

}