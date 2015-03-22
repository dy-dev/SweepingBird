#include <stdio.h>
#include <string.h>
#include <iostream>

#include <glew/glew.h>
#include <imgui/imgui.h>
#include <imgui/imguiRenderGL3.h>

#include <GLFW/glfw3.h>

#include <ProgramGUI.h>
#include <Camera.h>

using namespace SweepingBirds;


// Font buffers
extern const unsigned char DroidSans_ttf[];
extern const unsigned int DroidSans_ttf_len;


ProgramGUI::ProgramGUI()
	:m_pWindow(nullptr),
	m_bIsStillRunning(true),
	m_iWidth(0),
	m_iHeight(0),
	m_iDPI(1)
{


}
ProgramGUI::ProgramGUI(int width, int height)
	: m_pWindow(nullptr),
	m_bIsStillRunning(true),
	m_iWidth(width),
	m_iHeight(height),
	m_iDPI(1)
{
}

ProgramGUI::~ProgramGUI()
{
	delete m_pWindow;
}

void ProgramGUI::init()
{
	init_GLFW();
	init_Glew();
}

void ProgramGUI::init_GLFW()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, GL_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Open a window and create its OpenGL context
	m_pWindow = glfwCreateWindow(m_iWidth / m_iDPI, m_iHeight / m_iDPI, "aogl", 0, 0);
	if (!m_pWindow)
	{
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(m_pWindow);
}

void ProgramGUI::init_Glew()
{
	if (m_pWindow)
	{
		// Init glew
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			exit(EXIT_FAILURE);
		}

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GL_TRUE);

		// Enable vertical sync (on cards that support it)
		glfwSwapInterval(1);
		GLenum glerr = GL_NO_ERROR;
		glerr = glGetError();

		if (glerr != GL_NO_ERROR && glerr != GL_INVALID_ENUM)
		{
			fprintf(stderr, "glerr.\n");
			exit(EXIT_FAILURE);
		}
		if (!imguiRenderGLInit(DroidSans_ttf, DroidSans_ttf_len))
		{
			fprintf(stderr, "Could not init GUI renderer.\n");
			exit(EXIT_FAILURE);
		}
	}
}


void ProgramGUI::event_loop_management()
{
	// Mouse states
	m_iLeftMouseButton = glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT);
	m_iRightMouseButton = glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_RIGHT);
	m_iMiddleMouseButton = glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_MIDDLE);

	if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		m_bIsStillRunning = false;
	}
}

void ProgramGUI::add_gui_element(std::string varName, GUIInfos* infos)
{
	auto element = m_mGUIElements.find(varName);
	if (element != m_mGUIElements.end())
	{
		element->second.first.push_back(infos);
	}
	else
	{
		std::vector<GUIInfos*> vec;
		vec.push_back(infos);
		m_mGUIElements[varName] = std::make_pair(vec, new bool(false));
	}
}

void ProgramGUI::display_gui(bool isDemo, double fps)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, m_iWidth, m_iHeight);
	if (!isDemo)
	{
		unsigned char mbut = 0;
		int mscroll = 0;
		double mousex;
		double mousey;
		glfwGetCursorPos(m_pWindow, &mousex, &mousey);
		mousex *= m_iDPI;
		mousey *= m_iDPI;
		mousey = m_iHeight - mousey;

		if (m_iLeftMouseButton == GLFW_PRESS)
			mbut |= IMGUI_MBUT_LEFT;

		imguiBeginFrame((int)mousex, (int)mousey, mbut, mscroll);
		
			
		char lineBuffer[512];
		imguiBeginScrollArea("SweepingBird", m_iWidth - 210, m_iHeight - 810, 200, 800, &mscroll);
		sprintf_s(lineBuffer, "FPS %f", fps);

		imguiLabel(lineBuffer);
		for each (auto infos in m_mGUIElements)
		{
			auto toggle = imguiCollapse(infos.first.c_str(), "", (*infos.second.second), infos.second.second);
			if (*infos.second.second)
			{
				imguiIndent();
				for each (auto elementInfo in infos.second.first)
				{
					if (elementInfo != nullptr && elementInfo->type == SLIDER)
					{
						for each (auto value in elementInfo->var)
						{
							imguiSlider(value.first.c_str(), value.second, elementInfo->min, elementInfo->max, elementInfo->step);
						}
					}
					else if (elementInfo != nullptr && elementInfo->type == CHECKBOX)
					{
						auto toggleChk = imguiCheck(elementInfo->name.c_str(), *elementInfo->check_adress);
						if (toggleChk)
							*elementInfo->check_adress = !(*elementInfo->check_adress);
					}
					else if (elementInfo != nullptr && elementInfo->type == BUTTON)
					{
						auto toggleChk = imguiButton(elementInfo->name.c_str());
						if (toggleChk)
							elementInfo->button_action(elementInfo->obj);
					}

				}
				imguiUnindent();
			}
			if (toggle)
				*infos.second.second = !(*infos.second.second);
		}
		imguiEndScrollArea();
		imguiEndFrame();
		imguiRenderGLDraw(m_iWidth, m_iHeight);
	}
	glDisable(GL_BLEND);
	glfwSwapBuffers(m_pWindow);
	glfwPollEvents();
}
