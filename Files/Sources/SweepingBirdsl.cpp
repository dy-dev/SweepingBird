#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <windows.h>

#include "glew/glew.h"

#include "GLFW/glfw3.h"
#include "stb/stb_image.h"
#include "imgui/imgui.h"
#include "imgui/imguiRenderGL3.h"

#include "glm/glm.hpp"
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4, glm::ivec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/gtc/type_ptr.hpp" // glm::value_ptr

#include <ProgramGUI.h>
#include <SceneManager.h>
#include <TextureManager.h>
#include <UtilityToolKit.h>

int main(int argc, char **argv)
{
	SceneManager MySceneManager;
	MySceneManager.init();

	ProgramGUI MainWindow(1024, 768);
	MainWindow.init();

	TextureManager textureManager;
	textureManager.generate_textures("brick","./textures/spnza_bricks_a_diff.tga", "./textures/spnza_bricks_a_spec.tga");
	textureManager.generate_textures("grass", "./textures/ground_grass_1024_tile.tga", "./textures/ground_grass_1024_tile_specular.tga");
	textureManager.generate_textures("stone", "./textures/stone_ground_diffuse.tga", "./textures/stone_ground_specular.tga");

	textureManager.create_deferred_texture("deferred", MainWindow.get_width(), MainWindow.get_height());

	textureManager.create_shadow_texture("shadow", MySceneManager.get_shadow_texture_size());

	MySceneManager.set_texture_manager(&textureManager);
	MySceneManager.set_programGUI(&MainWindow);
	MySceneManager.setup_lights();
	MySceneManager.setup_shader_programs();
	MySceneManager.setup_frame_buffer();
	MySceneManager.setup_objects();

	do
	{
		MainWindow.event_loop_management();
		MySceneManager.set_cam_states();
		MySceneManager.manage_camera_movements();
		MySceneManager.display_scene(true);
		MainWindow.display_gui();
		
		// Check for errors
		UtilityToolKit::check_errors("End loop");
	} // Check if the ESC key was pressed
	while (MainWindow.is_still_running());

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
