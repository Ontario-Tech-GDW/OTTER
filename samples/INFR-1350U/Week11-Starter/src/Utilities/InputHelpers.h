#pragma once

#include <functional>

struct GLFWwindow;

/// <summary>
/// Helper class for watching a single key and invoking a method on the first frame that it is
/// pressed
/// </summary>
struct KeyPressWatcher final
{
public:
	/// <summary>
	/// Creates a new key press watcher with a given key code and callback
	/// </summary>
	/// <param name="keycode">The GLFW key to watch</param>
	/// <param name="onPressed">The function to invoke on the first frame the key is pressed</param>
	KeyPressWatcher(int keycode, const std::function<void()>& onPressed);
	~KeyPressWatcher() = default;

	bool Poll(GLFWwindow* window) const;
	
protected:
	int _keyCode;
	mutable bool _isPressed;
	std::function<void()> _onPressed;
};