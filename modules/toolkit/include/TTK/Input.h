#pragma once
#include <GLM/detail/type_vec2.hpp>
#include "../EnumToString.h"

namespace TTK {

	ENUM(KeyCode, uint16_t,
		Space = 32,
		Apostrophe = 39,
		Comma = 44,
		Minus = 45,
		Period = 46,
		Slash = 47,
		Zero = 48,
		One = 49,
		Two = 50,
		Three = 51,
		Four = 52,
		Five = 53,
		Six = 54,
		Seven = 55,
		Eight = 56,
		Nine = 57,
		Semicolon = 59,
		Equal = 61,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LeftBracket = 91,
		Backslash = 92,
		RightBracket = 93,
		GraveAccent = 96,
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
		KP_0 = 320,
		KP_1 = 321,
		KP_2 = 322,
		KP_3 = 323,
		KP_4 = 324,
		KP_5 = 325,
		KP_6 = 326,
		KP_7 = 327,
		KP_8 = 328,
		KP_9 = 329,
		KP_Decimal = 330,
		KP_Divide = 331,
		KP_Multiply = 332,
		KP_Subtract = 333,
		KP_Add = 334,
		KP_Enter = 335,
		KP_Equal = 336,
		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	);
		
	#define TTK_KEY_LAST TTK::KeyCode::Menu
	#define TTK_KEY_FIRST TTK::KeyCode::Space

	/*
	 * Represents the current state of a button
	 */
	ENUM(ButtonState, uint8_t,
		Up       = 0b00,
		Pressed  = 0b01,
		Down     = 0b11,
		Released = 0b10,
	)
	
	ENUM(MouseButton, uint8_t,
		Left,
		Right,
		Middle,
		Alt4,
		Alt5,
		Alt6,
		Alt7,
		Alt8,
	)

	#define TTK_MOUSEBUTTON_LAST TTK::MouseButton::Alt8
	#define TTK_MOUSEBUTTON_FIRST TTK::MouseButton::Left
	
	class Input {
	public:
		virtual ~Input() = default;

		/*
		 * Checks if the given key is currently being pressed
		 * @param key The key to check the state of
		 * @returns True if the key is pressed, false if not
		 */
		static bool GetKeyDown(KeyCode key);
		/*
		 * Checks if the given key has been newly pressed, as of the last time
		 * Poll has been called
		 * @param key The key to check the state of
		 * @returns True if the key is down this frame, but up in the previous frame
		 */
		static bool GetKeyPressed(KeyCode key);
		/*
		 * Checks if the given key has been newly released, as of the last time
		 * Poll has been called
		 * @param key The key to check the state of
		 * @returns True if the key is up this frame, but down in the previous frame
		 */
		static bool GetKeyReleased(KeyCode key);
		/*
		 * Gets the current key state of a given keyboard key
		 * @param key The key to get the state for
		 * @returns A ButtonState that represents the key's current state
		 */
		static ButtonState GetKeyState(KeyCode key);

		/*
		 * Gets the current x position on the mouse on the screen, in screen-space
		 * coordinates. 0,0 is in the top-left of the window
		 */
		static float GetMouseX();
		/*
		 * Gets the current y position on the mouse on the screen, in screen-space
		 * coordinates. 0,0 is in the top-left of the window
		 */
		static float GetMouseY();
		/*
		 * Gets the current position on the mouse on the screen, in screen-space
		 * coordinates. 0,0 is in the top-left of the window
		 */
		static glm::vec2 GetMousePos();
		/*
		 * Gets the current value of the mouse scroll wheel
		 */
		static glm::vec2 GetMouseScroll();
		/*
		 * Gets the change in the scroll wheel since the last time Poll was called
		 */
		static glm::vec2 GetMouseScrollDelta();
		/*
		 * Gets the current state of a mouse button
		 * @param button The mouse button to check the state of
		 * @returns True if the given button is currently down, false if otherwise
		 */
		static bool GetMouseDown(MouseButton button);
		/*
		 * Gets whether the given mouse button is newly pressed as of this frame
		 * @param button The mouse button to check the state of
		 * @returns True if the given button is currently down, and was up the previous frame
		 */
		static bool GetMousePressed(MouseButton button);
		/*
		 * Gets whether the given mouse button is newly released as of this frame
		 * @param button The mouse button to check the state of
		 * @returns True if the given button is currently up, and was down the previous frame
		 */
		static bool GetMouseReleased(MouseButton button);
		/*
		 * Gets the current key state of a given mouse key
		 * @param key The key to get the state for
		 * @returns A ButtonState that represents the key's current state
		 */
		static ButtonState GetMouseState(MouseButton button);

		/*
		 * Sets up the TTK input systems
		 * @param windowPtr The pointer to the window to use for the input system, either a GLFW or GLUT window
		 */
		static void Init(void* windowPtr);
		/*
		 * Cleans up the TTK input systems
		 */
		static void Uninitialize();
		/*
		 * Allows the TTK input system to check for changes in input devices, this should be called at the end of a
		 * frame, before glfwPollEvents
		 */
		static void Poll();

	private:
		static Input* m_Instance;

	protected:
		Input() = default;
		
		virtual ButtonState  __GetKeyState(KeyCode key) = 0;
		virtual ButtonState __GetMouseState(MouseButton button) = 0;
		
		virtual glm::vec2 __GetMousePos() = 0;
		virtual glm::vec2 __GetMouseScroll() = 0;
		virtual glm::vec2 __GetMouseScrollDelta() = 0;
		virtual void __Poll() = 0;
		virtual void __Init(void* windowPtr) = 0;
	};
}
