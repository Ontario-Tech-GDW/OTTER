#pragma once

#ifdef TTK_GLFW

#include "Input.h"
#include "GLFW/glfw3.h"

namespace TTK {
	class GlfwInput : public Input {
	public:
		GlfwInput();
		virtual ~GlfwInput();
		
	protected:
		ButtonState __GetKeyState(KeyCode key) override;
		ButtonState __GetMouseState(MouseButton button) override;
		glm::vec2 __GetMousePos() override;
		glm::vec2 __GetMouseScroll() override;
		glm::vec2 __GetMouseScrollDelta() override;
		void __Poll() override;
		void __Init(void* windowPtr) override;

		GLFWwindow* m_Window;
		char m_PrevKeys[(size_t)TTK_KEY_LAST + 1];
		char m_PrevMouse[(size_t)TTK_MOUSEBUTTON_LAST + 1];
	};
}
#endif
