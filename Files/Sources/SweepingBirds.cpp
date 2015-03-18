#pragma warning(disable : 4201)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <windows.h>

#include <glew/glew.h>

#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <imgui/imgui.h>
#include <imgui/imguiRenderGL3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <thread>

#include <ProgramGUI.h>
#include <SceneManager.h>
#include <TextureManager.h>
#include <UtilityToolKit.h>

#include <PhysicsEngine.h>

using namespace SweepingBirds;


#include <Skybox.h>
#include <ogldev_math_3d.h>

OVGCamera* m_pGameCamera;
PersProjInfo m_persProjInfo;
SweepingBirds::SkyBox * m_pSkyBox;

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	bool bIsDemoProgram = false;
	ProgramGUI MainWindow(1024, 768);
	MainWindow.init();

	SceneManager MySceneManager;
	MySceneManager.set_programGUI(&MainWindow);
	TextureManager textureManager;
	MySceneManager.set_texture_manager(&textureManager);

	MySceneManager.init();

	MySceneManager.setup_lights();
	MySceneManager.setup_shader_programs();
	MySceneManager.setup_frame_buffer();
	//MySceneManager.setup_skybox();
	MySceneManager.setup_objects();

	PhysicsEngine MyPhysicsEngine(&MySceneManager);
	MyPhysicsEngine.set_programGUI(&MainWindow);

	if (bIsDemoProgram)
	{
		MySceneManager.setupdemo();
		std::thread demothread(MySceneManager.demo, &MySceneManager, MainWindow.get_time());
	}

	float elapsedTime = 0.f;
	do
	{
		MainWindow.event_loop_management();

		MyPhysicsEngine.update(elapsedTime);//must be called after event_loop_management because of time synchro
	
		MySceneManager.set_cam_states();
		MySceneManager.manage_camera_movements();
		MySceneManager.display_scene(false);
		
		MainWindow.display_gui(bIsDemoProgram);

		// Check for errors
		UtilityToolKit::check_errors("End loop");

		elapsedTime = static_cast<float>(glfwGetTime()) - elapsedTime;
	} // Check if the ESC key was pressed
	while (MainWindow.is_still_running());

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
