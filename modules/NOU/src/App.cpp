/*
NOU Framework - Created by Samantha Stahlke for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

App.cpp
Minimal utility class for GLFW window management.
*/

#include "NOU/App.h"
#include "NOU/Input.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include "imgui_impl_opengl3.cpp"
#include "imgui_impl_glfw.cpp"

#include "glad/glad.h"

#include <iostream>

namespace nou
{
	GLFWwindow* App::m_window = nullptr;
	float App::m_prevTime = 0.0f;
	float App::m_deltaTime = 0.0f;
	bool App::m_imguiInit = false;

	//Creates our GLFW window.
	void App::Init(const std::string& name, int width, int height)
	{
		if (glfwInit() == GLFW_FALSE)
		{
			std::cout << "GLFW init failed!" << std::endl;
			throw std::runtime_error("GLFW init failed!");
		}

		//This will let us clear the window with transparency if we want (so we
		//can see other apps through our window). Which is pretty neato.
		//Set to true if you want to play with this - you'll probably want to 
		//adjust your blending function, as a heads-up.
		//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

		//This will lock the size of our window.
		//Typically you will not let users resize your game arbitrarily.
		//There are lots of reasons for this:
		//- Scaling your UI appropriately to an arbitrary aspect ratio is challenging.
		//- Anything dependent on your camera's current projection will also be affected.
		//- Once you get into post-processing effects, resizing may actually take a significant
		//amount of time (i.e., a second or more) - so resizing a window by dragging is a no-no.
		glfwWindowHint(GLFW_RESIZABLE, false);
		
		m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

		//This tells OpenGL we want to draw to the window we just created.
		//If you had multiple windows, you'd be calling this on each one before
		//making draw calls each frame.
		glfwMakeContextCurrent(m_window);
		
		//This tells GLFW what function we'd like to use to process its input messages.
		//(In our case, a static function in the Input class.)
		glfwSetKeyCallback(m_window, Input::GLFWInputCallback);

		//This initializes OpenGL via GLAD.
		if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
		{
			std::cout << "Glad init failed!" << std::endl;
			throw std::runtime_error("Glad init failed!");
		}

		printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
		printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

		//Some default GL settings.

		//This one makes it so that we can't draw anything on top of something 
		//that should be in front of it (e.g., our background doesn't accidentally
		//get drawn on top of our main character).
		glEnable(GL_DEPTH_TEST);

		//This one makes it so that we won't draw the "back faces" of an object.
		//(In other words, the stuff we wouldn't be able to see for opaque objects anyway.)
		glEnable(GL_CULL_FACE);

		//This one controls how semi-transparent objects will be blended.
		//If you start playing with alpha textures and things don't look right,
		//or you want a specific behaviour, you'll want to play with these parameters.
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//This initializes the background colour we want to use to clear our window.
		//This default is black.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void App::InitImgui()
	{
		if (m_imguiInit)
			return;

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.IniFilename = NULL;

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_TransparentBackbuffers;

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		//Blaze it, my dudes.
		ImGui_ImplOpenGL3_Init("#version 420");

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.8f;

		m_imguiInit = true;
	}

	void App::Cleanup()
	{
		if (m_imguiInit)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void App::Tick()
	{
		float time = static_cast<float>(glfwGetTime());
		m_deltaTime = time - m_prevTime;
		m_prevTime = time;
	}

	void App::FrameStart()
	{
		//Calculate our delta time for this frame.
		Tick();

		//Input polling.
		Input::FrameStart();
		glfwPollEvents();

		//Clear our window.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void App::SwapBuffers()
	{
		//This will post the results of all our draw calls to the window.
		glfwSwapBuffers(m_window);
	}

	void App::StartImgui()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void App::EndImgui()
	{
		ImGuiIO& io = ImGui::GetIO();
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);
		io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) 
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(m_window);
		}
	}

	float App::GetDeltaTime()
	{
		return m_deltaTime;
	}

	bool App::IsClosing()
	{
		return glfwWindowShouldClose(m_window);
	}

	void App::SetClearColor(const glm::vec4& clearColor)
	{
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	}
}
