#include <Ground.h>

using namespace SweepingBirds;

Ground::Ground()
{
}


Ground::~Ground()
{
}

float Ground::getGroundHeight()
{
	float ret = 0.f;
	ret = *(m_Ground3D.get_y_pos());
	
	return ret;
}
