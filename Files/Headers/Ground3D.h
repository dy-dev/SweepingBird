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

		float * get_height() { return &m_fHeight; }
		void set_height(float height) { m_fHeight = height; }

		virtual void draw(ShaderProgramManager& shaderMgr, Camera * cam, glm::mat4 proj, float time, int nbInstance) override;

	private:
		float m_fHeight;
		float m_fMountainFrequency;
		float m_iPatchSize;
	};

}