#include "Camera.h"
#define M_PI 3.14f


Camera::Camera()
{
}


Camera::~Camera()
{
}


void Camera::Camera_compute()
{
	eye.x = cos(theta) * sin(phi) * radius + o.x;
	eye.y = cos(phi) * radius + o.y;
	eye.z = sin(theta) * sin(phi) * radius + o.z;
	up = glm::vec3(0.f, phi < M_PI ? 1.f : -1.f, 0.f);
}

void Camera::Camera_defaults()
{
	phi = 1.1405f;
	theta = 1.92f;
	radius = 2200;
	Camera_compute();
}

void Camera::Camera_zoom(float factor)
{
	radius += factor * radius;
	if (radius < 0.1)
	{
		radius = 10.f;
		o = eye + glm::normalize(o - eye) * radius;
	}
	Camera_compute();
}

void Camera::Camera_turn(float ph, float th)
{
	theta += 1.f * th;
	phi -= 1.f * ph;
	if (phi >= (2 * M_PI) - 0.1)
		phi = 0.00001f;
	else
		if (phi <= 0)
			phi = 2.f * M_PI - 0.1f;
	Camera_compute();
}

void Camera::Camera_pan(float x, float y)
{
	glm::vec3 up(0.f, phi < M_PI ? 1.f : -1.f, 0.f);
	glm::vec3 fwd = glm::normalize(o - eye);
	glm::vec3 side = glm::normalize(glm::cross(fwd, up));
	up = glm::normalize(glm::cross(side, fwd));
	o[0] += up[0] * y * radius * 2;
	o[1] += up[1] * y * radius * 2;
	o[2] += up[2] * y * radius * 2;
	o[0] -= side[0] * x * radius * 2;
	o[1] -= side[1] * x * radius * 2;
	o[2] -= side[2] * x * radius * 2;
	Camera_compute();
}
