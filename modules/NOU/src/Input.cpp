/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Input.cpp
Simple utility class for managing basic keyboard input.
*/

#include "NOU/Input.h"

#include <string>

namespace nou
{
	bool Input::m_keyStates[Input::MAX_KEYS];
	Input::KeyFlag Input::m_keyFlags[Input::MAX_KEYS];

	//This is registered by the App class as a callback for GLFW to send us keystroke information.
	void Input::GLFWInputCallback(GLFWwindow* win, int key, int scancode, int action, int mods)
	{
		if(key < 0 || key >= Input::MAX_KEYS)
			return;

		switch (action)
		{
			case GLFW_PRESS:
			m_keyFlags[key] = KeyFlag::PRESSED;
			m_keyStates[key] = true;
			break;

			case GLFW_RELEASE:
			m_keyFlags[key] = KeyFlag::RELEASED;
			m_keyStates[key] = false;
			break;

			default:
			break;
		}
	}

	void Input::Init()
	{
		memset(&m_keyStates, 0, MAX_KEYS * sizeof(bool));
		memset(&m_keyFlags, 0, MAX_KEYS * sizeof(KeyFlag));
	}

	//We want to clear our input flags at the beginning of every frame.
	//(e.g., to wipe any messages for keypresses/releases that happened last frame).
	void Input::FrameStart()
	{
		memset(&m_keyFlags, 0, MAX_KEYS * sizeof(KeyFlag));
	}

	bool Input::GetKey(int keycode)
	{
		if (keycode < 0 || keycode >= MAX_KEYS)
			return false;

		return m_keyStates[keycode];
	}

	bool Input::GetKeyDown(int keycode)
	{
		if (keycode < 0 || keycode >= MAX_KEYS)
			return false;

		return m_keyFlags[keycode] == KeyFlag::PRESSED;
	}

	bool Input::GetKeyUp(int keycode)
	{
		if (keycode < 0 || keycode >= MAX_KEYS)
			return false;

		return m_keyFlags[keycode] == KeyFlag::RELEASED;
	}
}