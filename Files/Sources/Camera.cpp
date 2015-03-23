#include <Camera.h>
#include <glm/vec3.hpp> // glm::mat4

#define M_PI 3.14f

using namespace SweepingBirds;

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
	theta = 0.0f;
	radius = 500;
	Camera_compute();
}

void Camera::Camera_zoom(float factor)
{
	radius += factor * radius;
	if (radius < 0.1)
	{
		radius = 10.f;
		//o = eye + glm::normalize(o - eye) * radius;
	}
	else if (radius > 10000.0f)
	{
		radius = 10000.0f;
	}
	while (cos(phi) * radius < -5.0f && radius > 100.0f)
	{
		phi -= 0.001;
	}
	Camera_compute();
}

void Camera::Camera_turn(float ph, float th)
{
	theta += 1.f * th;
	phi -= 1.f * ph;
	if (phi >= (4.0f*M_PI / 6.0f))
		phi = 4.0f*M_PI / 6.0f - 0.001f;
	else
		if (phi <= 0)
			phi = 0.001f;

	while (cos(phi) * radius < 1.0f && radius > 100.0f)
	{
		phi -= 0.001;
	}
	Camera_compute();
}

void Camera::Camera_pan(float x, float y)
{
	glm::vec3 up(0.f, phi < M_PI ? 1.f : -1.f, 0.f);
	glm::vec3 fwd = glm::normalize(o - eye);
	glm::vec3 side = glm::normalize(glm::cross(fwd, up));
	up = glm::normalize(glm::cross(side, fwd));

	o[0] -= fwd[0] * y * 2;
	//o[1] += fwd[1] * y * 2;
	o[2] -= fwd[2] * y * 2;
	o[0] += side[0] * x * 2;
	//o[1] -= side[1] * x * 2;
	o[2] += side[2] * x * 2;
	/*
	auto tmptranseyex = eye + x*side;
	auto tmptranseyey = eye + y*up;
	auto xvar = glm::dot(eye, tmptranseyex);
	xvar /= glm::length(eye) * glm::length(tmptranseyex);
	if (xvar < 1.0f)
	{
	auto mul = x < 0.0f ? 1.f : -1.f;
	auto tmpangle = mul*acos(xvar);
	radius += sqrt(xvar*xvar/ radius*cos(phi) + 1 );
	theta += tmpangle;
	}
	auto yvar = glm::dot(eye, tmptranseyey);
	yvar /= glm::length(eye) * glm::length(tmptranseyey);
	Camera_compute();
	if (yvar < 1.0f)
	{
	auto mul = y < 0.0f ? 1.f : -1.f;
	radius = glm::length(tmptranseyey);
	phi += mul*acos(yvar);
	}*/
	Camera_compute();
}

void Camera::jump_to_pos(glm::vec3 position, glm::vec3 direction)
{
	o = position;

	radius = 300;

	eye = o - /*glm::vec3(300)*/ radius *direction;
  m_matTransform = glm::lookAt(eye, o, glm::vec3(0, 1, 0));
	auto val = glm::dot(glm::normalize(direction), glm::vec3(0.0, 1.0, 0.0));
	phi = acos(val);
	Camera_compute_angles();
}

void Camera::Camera_compute_angles()
{

	auto val = glm::dot(glm::normalize(eye), glm::vec3(0.0, 1.0, 0.0));
	phi = acos(val);

	if (sin(phi) != 0)
	{
		auto tmp = glm::normalize(eye);
		theta = acos(tmp.x / sin(phi));
	}
	else
	{
		theta = 0;
	}
	if (abs(cos(theta) * sin(phi) * radius + o.x - eye.x) > 0.1)
	{
		if (abs(sin(theta) * sin(phi) * radius + o.z - eye.z) > 0.1)
		{
			if (theta > M_PI / 2)
				theta -= M_PI;
			else
				theta += M_PI;
		}
		else
		{
			theta = -theta;
		}
	}
}
