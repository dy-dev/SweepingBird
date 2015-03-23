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

		const Ground3D & get_ground_3d() const { assert(m_Ground3D); return *m_Ground3D; };
		void set_ground_3d(const Ground3D* newGround) { m_Ground3D = newGround; }
		float get_ground_height();
		float get_mountain_frequency() { assert(m_Ground3D); return m_Ground3D->get_frequency(); }

		float Noise(int x, int y);
		float Interpolate(float x, float y, float a);
		float GetValue(float x, float y);
		float Total(float i, float j);

	private:
		const Ground3D* m_Ground3D;

		float m_fMountainFrequency;
	};
}