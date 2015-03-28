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
    /**
     * @fn  Camera::Camera();
     *
     * @brief Default constructor.
     */
		Camera();
		~Camera();

    /**
     * @fn  void Camera::Camera_compute();
     *
     * @brief Camera compute.
     */
		void Camera_compute();

    /**
     * @fn  void Camera::Camera_compute_angles();
     *
     * @brief Camera compute angles.
     */
		void Camera_compute_angles();

    /**
     * @fn  void Camera::Camera_defaults();
     *
     * @brief Camera defaults.
     */
		void Camera_defaults();

    /**
     * @fn  void Camera::Camera_zoom(float factor);
     *
     * @brief Camera zoom.
     *
     * @param factor  The factor.
     */
		void Camera_zoom(float factor);

    /**
     * @fn  void Camera::Camera_turn(float phi, float theta);
     *
     * @brief Camera turn.
     *
     * @param phi   The phi.
     * @param theta The theta.
     */
		void Camera_turn(float phi, float theta);

    /**
     * @fn  void Camera::Camera_pan(float x, float y);
     *
     * @brief Camera pan.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
		void Camera_pan(float x, float y);

    /**
     * @fn  glm::vec3 Camera::GetO() const
     *
     * @brief Gets the o.
     *
     * @return  The o.
     */
		glm::vec3 GetO() const { return o; };

    /**
     * @fn  glm::vec3 Camera::GetEye() const
     *
     * @brief Gets the eye.
     *
     * @return  The eye.
     */
		glm::vec3 GetEye() const { return eye; };

    /**
     * @fn  glm::vec3 Camera::GetUp() const
     *
     * @brief Gets the up.
     *
     * @return  The up.
     */
		glm::vec3 GetUp() const { return up; };

    /**
     * @fn  void Camera::jump_to_pos(glm::vec3 position, glm::vec3 direction, float angle);
     *
     * @brief Jump to position.
     *
     * @param position  The position.
     * @param direction The direction.
     * @param angle     The angle.
     */
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