#pragma once
#include <Textured3DObject.h>
namespace SweepingBirds
{
	class Camera;
	class ObjectManager;
	class Bird3D : public Textured3DObject
	{
	public:

		static void stick_cam(void *obj);

		Bird3D();
		Bird3D(ObjectManager* manager, TextureManager * texMgr);
		~Bird3D();

		void  set_speed(float speed) { m_fSpeed = speed; }
		float * get_speed() { return &m_fSpeed; }

		void set_direction(glm::vec3 Direction) { m_v3Direction = Direction; };
		glm::vec3 get_direction(){ return m_v3Direction; };

		virtual void draw(ShaderProgramManager& shaderMgr, glm::mat4 proj, float time, int nbInstance) override;
		

	private:
		float m_fSpeed;

		
	};

}