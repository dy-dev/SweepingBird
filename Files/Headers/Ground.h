#pragma once
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#include <Ground3D.h>

namespace SweepingBirds
{
	// the actual thread pool
	class  ThreadPool
	{
		friend class Ground;

	public:
		ThreadPool(size_t);
		template<class F, class... Args>
		auto enqueue(F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type > ;
		~ThreadPool();
	private:
		friend class Worker;

		// need to keep track of threads so we can join them
		std::vector< std::thread > workers;

		// the task queue
		std::queue< std::function<void()> > tasks;

		// synchronization
		std::mutex queue_mutex;
		std::condition_variable condition;

		bool stop;
	};

	// our worker thread objects
	class Worker
	{
	public:
		Worker(ThreadPool &s) : pool(s) { }
		void operator()();
	private:
		ThreadPool &pool;
	};

	class Ground3D;
	class Ground
	{
	private:
		static void generate_H_map(Ground* ground, int zOffset, int xOffset, int patchIndex, int patchArea, const std::vector<float>& verts);
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
		const Ground3D & get_ground_3d() const { assert(m_pGround3D); return *m_pGround3D; };

		/**
		 * @fn  void Ground::set_ground_3d(const Ground3D* newGround)
		 *
		 * @brief Sets ground 3D.
		 *
		 * @param newGround The new ground.
		 */
		void set_ground_3d(Ground3D* newGround);

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
		float get_mountain_frequency() { assert(m_pGround3D); return m_pGround3D->get_frequency(); }

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
		void generate_heigh_map(int nbPatch);
		void generate_normals(Ground* ground, int patchIndex);

	private:
		Ground3D* m_pGround3D;

		float m_fMountainFrequency;
		float* heightMap;
		int heightMapSize;
		float* normalMap;
		int normalMapSize;

		ThreadPool * m_pTPool;

		std::vector<std::future<void>> futures;
	};
}