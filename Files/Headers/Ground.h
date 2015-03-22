#pragma once
#include <Ground3D.h>

namespace SweepingBirds
{
	class Ground3D;
	class Ground
	{

	public:
		Ground();
		~Ground();

		const Ground3D & get_ground() const { return m_Ground3D; };
		float getGroundHeight();
		float getMountainFrequency() { return m_fMountainFrequency; }

	private:
		Ground3D m_Ground3D;

		float m_fMountainFrequency;
	};
}