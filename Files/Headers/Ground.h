#pragma once
#include <Ground3D.h>

namespace SweepingBirds
{
	class Ground3D;
	class Ground
	{

	public:
    /**
     * @fn  Ground::Ground();
     *
     * @brief Default constructor.
     */
		Ground();
		~Ground();

    /**
     * @fn  const Ground3D & Ground::get_ground_3d() const
     *
     * @brief Gets ground 3D.
     *
     * @return  The ground 3D.
     */
		const Ground3D & get_ground_3d() const { assert(m_Ground3D); return *m_Ground3D; };

    /**
     * @fn  void Ground::set_ground_3d(const Ground3D* newGround)
     *
     * @brief Sets ground 3D.
     *
     * @param newGround The new ground.
     */
		void set_ground_3d(const Ground3D* newGround) { m_Ground3D = newGround; }

    /**
     * @fn  float Ground::get_ground_height();
     *
     * @brief Gets ground height.
     *
     * @return  The ground height.
     */
		float get_ground_height();

    /**
     * @fn  float Ground::get_mountain_frequency()
     *
     * @brief Gets mountain frequency.
     *
     * @return  The mountain frequency.
     */
		float get_mountain_frequency() { assert(m_Ground3D); return m_Ground3D->get_frequency(); }

    /**
     * @fn  float Ground::Noise(int x, int y);
     *
     * @brief Noises.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     *
     * @return  A float.
     */
		float Noise(int x, int y);

    /**
     * @fn  float Ground::Interpolate(float x, float y, float a);
     *
     * @brief Interpolates.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @param a The float to process.
     *
     * @return  A float.
     */
		float Interpolate(float x, float y, float a);

    /**
     * @fn  float Ground::GetValue(float x, float y);
     *
     * @brief Gets a value.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     *
     * @return  The value.
     */
		float GetValue(float x, float y);

    /**
     * @fn  float Ground::Total(float i, float j);
     *
     * @brief Totals.
     *
     * @param i Zero-based index of the.
     * @param j The float to process.
     *
     * @return  A float.
     */
		float Total(float i, float j);
		void generate_heigh_map(float xmin, float xmax, float zmin, float zmax);
		void generate_normals(std::vector<float>& heightmap, std::vector<float>& normalmap);

	private:
		Ground3D* m_pGround3D;

		float m_fMountainFrequency;
	};
}