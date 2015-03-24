#pragma once
#include <Textured3DObject.h>
namespace SweepingBirds
{
	class Camera;
	class ObjectManager;
	class GPUBuffer;
	class Ground3D : public Textured3DObject
	{
	public:
		static const GLuint HEIGHTMAP_BINDING;
		static const GLuint NORMALMAP_BINDING;
	public:
		Ground3D();
		Ground3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance);
		~Ground3D();

		const float& get_height() const { return m_fHeight; }
		void set_height(float height) { m_fHeight = height; }
		const float& get_frequency() const { return m_fMountainFrequency; }

		void update(const std::vector<float>& heightdatas, const std::vector<float>& normaldatas);
		virtual void draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;

	private:
		float m_fHeight;
		float m_fMountainFrequency;
	public:
		float m_iPatchSize;
		float m_ftest1;
		float m_ftest2;

		GPUBuffer * m_bufHeightMapGPUBuffer;
		GPUBuffer * m_bufNormalMapGPUBuffer;
	};

}