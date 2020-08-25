#include "TTK/GLFWInput.h"
#include "GLFW/glfw3.h"
#include "Logging.h"

struct NativeWindowProxy {
	void* spacer0;
	int   spacer1[6];
	void* userPointer;
	GLFWvidmode videoMode;
	void* spacer2[2];
	int   spacer3[10];
	char  mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
	char  keys[GLFW_KEY_LAST + 1];
};

GLFWscrollfun g_PrevScrollCallback = nullptr;
glm::dvec2    g_CurrentMouseScroll = glm::dvec2(0.0);
glm::dvec2    g_CurrentMouseScrollDelta = glm::dvec2(0.0);

void __HandleMouseScroll(GLFWwindow* window, double xDiff, double yDiff) {
	g_CurrentMouseScrollDelta.x += xDiff;
	g_CurrentMouseScrollDelta.y += yDiff;
	g_CurrentMouseScroll.x += xDiff;
	g_CurrentMouseScroll.y += yDiff;

	// Callback chaining
	if (g_PrevScrollCallback)
		g_PrevScrollCallback(window, xDiff, yDiff);
}

TTK::GlfwInput::GlfwInput() : TTK::Input() {
	memset(m_PrevKeys, 0, sizeof(m_PrevKeys));
	memset(m_PrevMouse, 0, sizeof(m_PrevMouse));
}

TTK::GlfwInput::~GlfwInput() = default;

TTK::ButtonState TTK::GlfwInput::__GetKeyState(KeyCode key) {
	return (TTK::ButtonState)(!!glfwGetKey(m_Window, (size_t)key) | (!!m_PrevKeys[(size_t)key] << 1));
}

TTK::ButtonState TTK::GlfwInput::__GetMouseState(MouseButton button) {
	return (TTK::ButtonState)(!!glfwGetMouseButton(m_Window, *button) | (!!m_PrevMouse[*button] << 1));
}

glm::vec2 TTK::GlfwInput::__GetMousePos() {
	glm::dvec2 pos;
	glfwGetCursorPos(m_Window, &pos.x, &pos.y);
	return pos;
}

glm::vec2 TTK::GlfwInput::__GetMouseScroll() {
	return g_CurrentMouseScroll;
}

glm::vec2 TTK::GlfwInput::__GetMouseScrollDelta() {
	return g_CurrentMouseScrollDelta;
}

void TTK::GlfwInput::__Poll() {
	NativeWindowProxy* proxy = (NativeWindowProxy*)m_Window;
	// Copy our mouse state and key state from our GLFW window proxy struct
	memcpy(m_PrevKeys, proxy->keys, sizeof(m_PrevKeys));
	memcpy(m_PrevMouse, proxy->mouseButtons, sizeof(m_PrevMouse));
	// Reset the scroll wheel delta
	g_CurrentMouseScrollDelta = { 0, 0 };
	// We will assume that glfwPollEvents has been called
}
void TTK::GlfwInput::__Init(void* windowPtr) {
	m_Window = (GLFWwindow*)windowPtr;
	memset(m_PrevKeys, 0, sizeof(m_PrevKeys));
	memset(m_PrevMouse, 0, sizeof(m_PrevMouse));

	g_PrevScrollCallback = glfwSetScrollCallback(m_Window, __HandleMouseScroll);
}
