#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

namespace SweepingBirds
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void Camera_compute();
		void Camera_compute_angles();
		void Camera_defaults();
		void Camera_zoom(float factor);
		void Camera_turn(float phi, float theta);
		void Camera_pan(float x, float y);

		glm::vec3 GetO() const { return o; };
		glm::vec3 GetEye() const { return eye; };
		glm::vec3 GetUp() const { return up; };
		void jump_to_pos(glm::vec3 position, glm::vec3 direction, float angle);

	protected:
		float radius;
		float theta;
		float phi;

		float m_fRotAngle;

		glm::vec3 o;
		glm::vec3 eye;
		glm::vec3 up;

	};

}