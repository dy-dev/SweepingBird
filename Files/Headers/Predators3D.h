#pragma once
#include <Textured3DObject.h>
#include <GPUBuffer.h>

namespace SweepingBirds
{
	class ObjectManager;
	class Camera;
	class Predators3D : public Textured3DObject
	{
	public:
		/** @brief The predators binding. */
		static const GLuint PREDATORS_BINDING;

    /**
     * @fn  Predators3D::Predators3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance);
     *
     * @brief Constructor.
     *
     * @param [in,out]  manager If non-null, the manager.
     * @param [in,out]  texMgr  If non-null, manager for tex.
     * @param nbInstance        The nb instance.
     */
		Predators3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance);
		~Predators3D();

    /**
     * @fn  virtual void Predators3D::draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;
     *
     * @brief Draws.
     *
     * @param [in,out]  shaderMgr Manager for shader.
     * @param proj                The project.
     * @param time                The time.
     * @param nbInstance          The nb instance.
     */
		virtual void draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;

    /**
     * @fn  void Predators3D::update_positions(const std::vector<glm::vec3>& newPositions);
     *
     * @brief Updates the positions described by newPositions.
     *
     * @param newPositions  The new positions.
     */
		void update_positions(const std::vector<glm::vec3>& newPositions);

	private:
		GPUBuffer m_bufPredatorsData;

	};

}