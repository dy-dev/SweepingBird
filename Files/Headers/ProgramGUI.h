#pragma once
#include <map>
#include <functional>

#include <GLFW/glfw3.h>

#include <vector>


namespace SweepingBirds
{

	enum GUIType
	{
		SLIDER,
		CHECKBOX,
		BUTTON
	};
	
	struct GUIInfos
	{
		class Camera;
		std::string name;
		GUIType type;
		std::vector< std::pair< std::string, float *> > var;
		bool * check_adress;
		float min;
		float max;
		float step;
		std::function<void(void*, bool)> button_action;
		void * obj;
		GUIInfos(std::string n, float mn, float mx, float stp)
		{
			name = n;
			type = SLIDER;
			min = mn;
			max = mx;
			step = stp;
		}
		GUIInfos(std::string n, GUIType ty)
		{
			name = n;
			type = ty;
		}
	};
	class ProgramGUI
	{
	public:
    /**
     * @fn  ProgramGUI::ProgramGUI();
     *
     * @brief Default constructor.
     */
		ProgramGUI();

    /**
     * @fn  ProgramGUI::ProgramGUI(int width, int height);
     *
     * @brief Constructor.
     *
     * @param width   The width.
     * @param height  The height.
     */
		ProgramGUI(int width, int height);
		~ProgramGUI();

    /**
     * @fn  void ProgramGUI::init();
     *
     * @brief Initialises this object.
     */
		void init();

    /**
     * @fn  int ProgramGUI::get_width() const
     *
     * @brief Gets the width.
     *
     * @return  The width.
     */
		int get_width() const { return m_iWidth; }

    /**
     * @fn  int ProgramGUI::get_height() const
     *
     * @brief Gets the height.
     *
     * @return  The height.
     */
		int get_height() const { return m_iHeight; }

    /**
     * @fn  GLFWwindow * ProgramGUI::get_window() const
     *
     * @brief Gets the window.
     *
     * @return  null if it fails, else the window.
     */
		GLFWwindow * get_window() const { return m_pWindow; }

    /**
     * @fn  int ProgramGUI::get_left_button_state() const
     *
     * @brief Gets left button state.
     *
     * @return  The left button state.
     */
		int get_left_button_state() const { return m_iLeftMouseButton; }

    /**
     * @fn  int ProgramGUI::get_middle_button_state() const
     *
     * @brief Gets middle button state.
     *
     * @return  The middle button state.
     */
		int get_middle_button_state() const { return m_iMiddleMouseButton; }

    /**
     * @fn  int ProgramGUI::get_right_button_state() const
     *
     * @brief Gets right button state.
     *
     * @return  The right button state.
     */
		int get_right_button_state() const { return m_iRightMouseButton; }

    /**
     * @fn  void ProgramGUI::event_loop_management();
     *
     * @brief Event loop management.
     */
		void event_loop_management();

    /**
     * @fn  void ProgramGUI::display_gui(bool isDemo, double fps);
     *
     * @brief Displays a graphical user interface.
     *
     * @param isDemo  true if this object is demo.
     * @param fps     The FPS.
     */
		void display_gui(bool isDemo, double fps);

    /**
     * @fn  bool ProgramGUI::is_still_running() const
     *
     * @brief Query if this object is still running.
     *
     * @return  true if still running, false if not.
     */
		bool is_still_running() const { return m_bIsStillRunning; }

    /**
     * @fn  void ProgramGUI::add_gui_element(std::string varName, GUIInfos* infos);
     *
     * @brief Adds a graphical user interface element to 'infos'.
     *
     * @param varName         Name of the variable.
     * @param [in,out]  infos If non-null, the infos.
     */
		void add_gui_element(std::string varName, GUIInfos* infos);

		bool m_bMustSickToBird;
		bool m_bMustStopFollowingBird;
		bool m_bMustStickToPredator;

	protected:
    /**
     * @fn  void ProgramGUI::init_GLFW();
     *
     * @brief Initialises the glfw.
     */
		void init_GLFW();

    /**
     * @fn  void ProgramGUI::init_Glew();
     *
     * @brief Initialises the glew.
     */
		void init_Glew();
		
		
		GLFWwindow * m_pWindow;
		int m_iWidth;
		int m_iHeight;

		bool m_bIsStillRunning;
		
		bool panLock;
		bool turnLock;
		bool zoomLock;


		int m_iLeftMouseButton;
		int m_iMiddleMouseButton;
		int m_iRightMouseButton;

		int m_iDPI;

		std::map<std::string, std::pair<std::vector<GUIInfos*>, bool*> > m_mGUIElements;
	};

}