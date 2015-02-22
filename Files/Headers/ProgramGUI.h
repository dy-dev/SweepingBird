#pragma once
#include <map>

#include "GLFW/glfw3.h"

#include <vector>

class GLFWWindow;
class Camera;

enum GUIType
{
	SLIDER,
	CHECKBOX
};
struct GUIInfos
{
	std::string name;
	GUIType type;
	std::vector< std::pair< std::string, float *> > var;
	bool * check_adress;
	float min;
	float max;
	float step;
	
	GUIInfos(std::string n, float mn, float mx, float stp)
	{
		name = n;
		type = SLIDER;
		min = mn;
		max = mx;
		step = stp;
	}
	GUIInfos(std::string n)
	{
		name = n;
		type = CHECKBOX;
	}
};

class ProgramGUI
{
public:
	ProgramGUI();
	ProgramGUI(int width, int height);
	~ProgramGUI();

	void init();
	int get_width() const { return m_iWidth; }
	int get_height() const { return m_iHeight; }
	GLFWwindow * get_window() const { return m_pWindow; }

	int get_left_button_state() const { return m_iLeftMouseButton; }
	int get_middle_button_state() const { return m_iMiddleMouseButton; }
	int get_right_button_state() const { return m_iRightMouseButton; }

	void event_loop_management();
	void display_gui(bool isDemo);
	bool is_still_running() const { return m_bIsStillRunning; }
	double get_time() { return m_dPrevTime; }

	void add_gui_element(std::string varName, GUIInfos* infos);

protected:
	void init_GLFW();
	void init_Glew();
	void init_gui_states();

	GLFWwindow * m_pWindow;
	int m_iWidth;
	int m_iHeight;

	bool m_bIsStillRunning;

	bool panLock;
	bool turnLock;
	bool zoomLock;
	
	int Cameras;
	bool playing;
	
	float m_fFPS;
	double m_dPrevTime;

	int m_iLeftMouseButton;
	int m_iMiddleMouseButton;
	int m_iRightMouseButton;

	float light1XPos;
	float light1YPos;
	float light1ZPos;

	const int m_iDPI;

	std::map<std::string, std::pair<std::vector<GUIInfos*>, bool*> > m_mGUIElements;
};

