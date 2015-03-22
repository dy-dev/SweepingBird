#pragma once
#include <Textured3DObject.h>
namespace SweepingBirds
{
	class Camera;
	class ObjectManager;
	class Ground3D : public Textured3DObject
	{
	public:
		Ground3D();
		Ground3D(ObjectManager* manager, TextureManager * texMgr, int nbInstance);
		~Ground3D();

		const float& get_height() const { return m_fHeight; }
		void set_height(float height) { m_fHeight = height; }
    const float& get_frequency() const { return m_fMountainFrequency; }

		virtual void draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;

	private:
		float m_fHeight;
		float m_fMountainFrequency;
		float m_iPatchSize;
	};

}