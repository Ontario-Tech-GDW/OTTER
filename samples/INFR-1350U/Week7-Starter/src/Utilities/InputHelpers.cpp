#include "InputHelpers.h"
#include <GLFW/glfw3.h>

KeyPressWatcher::KeyPressWatcher(int keycode, const std::function<void()>& onPressed) {
	_isPressed = false;
	_keyCode = keycode;
	_onPressed = onPressed;
}

bool KeyPressWatcher::Poll(GLFWwindow* window) const {
	if (glfwGetKey(window, _keyCode) == GLFW_PRESS) {
		if (!_isPressed) {
			_isPressed = true;
			if (_onPressed) {
				_onPressed();
			}
		}
		return false;
	} else {
		_isPressed = false;
	}
	return false;
}
