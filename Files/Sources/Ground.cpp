#include <Ground.h>

using namespace SweepingBirds;

Ground::Ground()
{
}


Ground::~Ground()
{
}

float Ground::get_ground_height()
{
  assert(m_Ground3D);
	return m_Ground3D->get_height();
}
