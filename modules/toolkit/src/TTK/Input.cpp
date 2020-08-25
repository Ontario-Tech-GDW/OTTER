#include "TTK/Input.h"
#include "Logging.h"

#ifdef TTK_GLFW
#include "TTK/GLFWInput.h"
#endif

TTK::Input* TTK::Input::m_Instance = nullptr;

#define PROXY(func, ...) \
LOG_ASSERT(m_Instance != nullptr, "TTK Input has not been initialized!"); \
return m_Instance->func(__VA_ARGS__);

#define CALL(func, ...) \
LOG_ASSERT(m_Instance != nullptr, "TTK Input has not been initialized!"); \
m_Instance->func(__VA_ARGS__)

bool TTK::Input::GetKeyDown(KeyCode key) {
	LOG_ASSERT(m_Instance != nullptr, "TTK Input has not been initialized!");
	return *m_Instance->__GetKeyState(key) & *TTK::ButtonState::Pressed;
}
bool TTK::Input::GetKeyPressed(KeyCode key) {
	LOG_ASSERT(m_Instance != nullptr, "TTK Input has not been initialized!");
	return m_Instance->__GetKeyState(key) == TTK::ButtonState::Pressed;
}
bool TTK::Input::GetKeyReleased(KeyCode key) {
	LOG_ASSERT(m_Instance != nullptr, "TTK Input has not been initialized!");
	return m_Instance->__GetKeyState(key) == TTK::ButtonState::Released;
}
TTK::ButtonState TTK::Input::GetKeyState(KeyCode key) { PROXY(__GetKeyState, key); }

bool TTK::Input::GetMouseDown(MouseButton button) {
	LOG_ASSERT(m_Instance != nullptr, "TTK Input has not been initialized!");
	return *m_Instance->__GetMouseState(button) & *TTK::ButtonState::Pressed;
}
bool TTK::Input::GetMousePressed(MouseButton button) {
	LOG_ASSERT(m_Instance != nullptr, "TTK Input has not been initialized!");
	return m_Instance->__GetMouseState(button) == TTK::ButtonState::Pressed;
}
bool TTK::Input::GetMouseReleased(MouseButton button) {
	LOG_ASSERT(m_Instance != nullptr, "TTK Input has not been initialized!");
	return m_Instance->__GetMouseState(button) == TTK::ButtonState::Released;
}
TTK::ButtonState TTK::Input::GetMouseState(MouseButton button) { PROXY(__GetMouseState, button); }

float TTK::Input::GetMouseX() {
	LOG_ASSERT(m_Instance != nullptr, "TTK Input has not been initialized!");
	return m_Instance->__GetMousePos().x;
}
float TTK::Input::GetMouseY() {
	LOG_ASSERT(m_Instance != nullptr, "TTK Input has not been initialized!");
	return m_Instance->__GetMousePos().y;
}
glm::vec2 TTK::Input::GetMousePos() { PROXY(__GetMousePos); }
glm::vec2 TTK::Input::GetMouseScroll() { PROXY(__GetMouseScroll); }
glm::vec2 TTK::Input::GetMouseScrollDelta() { PROXY(__GetMouseScrollDelta); }

void TTK::Input::Poll() { PROXY(__Poll); }

void TTK::Input::Init(void* windowPtr) {
#ifdef TTK_GLFW
	m_Instance = new TTK::GlfwInput();
#elif TTK_GLUT
	m_instance = new TTK::GlutInput();
#else
	static_assert("Must specify either TTK_GLFW or TTK_GLUT!");
#endif
	m_Instance->__Init(windowPtr);
}

void TTK::Input::Uninitialize() {
	LOG_ASSERT(m_Instance != nullptr, "TTK Input has not been initialized!");
	delete m_Instance;
	m_Instance = nullptr;
}
