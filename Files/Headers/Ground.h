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

		const Ground3D & get_ground_3d() const { assert(m_pGround3D); return *m_pGround3D; };
		void set_ground_3d(Ground3D* newGround) { m_pGround3D = newGround; }
		float get_ground_height();
		float get_mountain_frequency() { assert(m_pGround3D); return m_pGround3D->get_frequency(); }

		float Noise(int x, int y);
		float Interpolate(float x, float y, float a);
		float GetValue(float x, float y);
		float Total(float i, float j);
		void generate_heigh_map(float xmin, float xmax, float zmin, float zmax);
		void generate_normals(std::vector<float>& heightmap, std::vector<float>& normalmap);

	private:
		Ground3D* m_pGround3D;

		float m_fMountainFrequency;
	};
}