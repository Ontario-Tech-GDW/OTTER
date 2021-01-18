//////////////////////////////////////////////////////////////////////////
//
// This header is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this header in your GDW games.
//
// This header contains utility functions for drawing.
//
// Michael Gharbharan 2017
// Shawn Matthews 2019
//
//////////////////////////////////////////////////////////////////////////

#ifndef GRAPHICS_UTILS_H
#define GRAPHICS_UTILS_H

#include <string>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace TTK
{
	/*
	 * Represents how the world is aligned, can be either Y up or Z up
	 */
	enum class AlignMode {
		ZUp = 0,
		YUp = 1
	};
	
	class Graphics
	{
	private:
		static inline float _fov = 60.0f;
		
	public:
		/*
		 * Draws the given text on the screen at the given coordinates
		 * @param text The text to draw
		 * @param posX The x position to draw the text at, in screen coordinates
		 * @param posY The y position to draw the text at, in screen coordinates
		 * @param fontSize The size of the text to draw, default is 16
		 */
		static void DrawText2D(const std::string& text, float posX, float posY, float fontSize = 16);
		/*
		 * Draws the given text on the screen at the given coordinates
		 * @param text The text to draw
		 * @param posX The x position to draw the text at, in screen coordinates
		 * @param posY The y position to draw the text at, in screen coordinates
		 * @param color The color of the text to draw
		 * @param fontSize The size of the text to draw, default is 16
		 */
		static void DrawText2D(const std::string& text, float posX, float posY, const glm::vec4& color, float fontSize = 16);

		/*
		 * Initializes ImGUI, using the given window
		 * @param window The root window of our game
		 */
		static void InitImGUI(GLFWwindow* window);
		/*
		 * Shuts down ImGUI and cleans up all of its resources
		 */
		static void ShutdownImGUI();

		/*
		 * Starts a new GUI frame, should be called every frame before any ImGUI calls
		 */
		static void BeginGUI();
		/*
		 * Ends a GUI frame, should be called every frame after all ImGUI calls
		 */
		static void EndGUI();
		
		/*
		 * Draws a line from p0 to p1, with the given color
		 * @param p0 The starting point of the line
		 * @param p1 The ending point of the line
		 * @param lineWidth[deprecated] The width of the line (Note that this is deprecated and only there to ensure consistency with old applications using TTK)
		 * @param colour The color of the line (rgba)
		 */
		[[deprecated("lineWidth is no longer a supported parameter, see overloads without lineWidth")]]
		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, float lineWidth = 1.0f, const glm::vec4& colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		/*
		 * Draws a line from p0 to p1, with the given color
		 * @param p0 The starting point of the line
		 * @param p1 The ending point of the line
		 * @param colour The color of the line (rgba)
		 */
		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4 & colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		/*
		 * Draws a line from p0 to p1, with the given color
		 * @param p0 The starting point of the line
		 * @param p1 The ending point of the line
		 * @param lineWidth[deprecated] The width of the line (Note that this is deprecated and only there to ensure consistency with old applications using TTK)
		 * @param colour The color of the line (rgba)
		 */
		[[deprecated("lineWidth is no longer a supported parameter, see overloads without lineWidth")]]
		static void DrawLine(float* p0, float* p1, float lineWidth = 1.0f, float *colour = nullptr);
		/*
		 * Draws a line from p0 to p1, with the given color
		 * @param p0 The starting point of the line
		 * @param p1 The ending point of the line
		 * @param colour The color of the line (rgba)
		 */
		static void DrawLine(float* p0, float* p1, float* colour = nullptr);

		/*
		 * Draws a vector, as a given point and direction
		 * @param origin The point to draw the vector from
		 * @param vectorToDraw The vector to draw
		 * @param lineWidth[deprecated] The width of the line (Note that this is deprecated and only there to ensure consistency with old applications using TTK)
		 * @param colour The color of the line (rgba)
		 */
		[[deprecated("lineWidth is no longer a supported parameter, see overloads without lineWidth")]]
		static void DrawVector(const glm::vec3& origin, const glm::vec3& vectorToDraw, float lineWidth = 1.0f, const glm::vec4& colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		/*
		 * Draws a vector, as a given point and direction
		 * @param origin The point to draw the vector from
		 * @param vectorToDraw The vector to draw
		 * @param colour The color of the line (rgba)
		 */
		static void DrawVector(const glm::vec3& origin, const glm::vec3& vectorToDraw, const glm::vec4 & colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		/*
		 * Draws a vector, as a given point and direction
		 * @param origin The point to draw the vector from
		 * @param vectorToDraw The vector to draw
		 * @param lineWidth[deprecated] The width of the line (Note that this is deprecated and only there to ensure consistency with old applications using TTK)
		 * @param colour The color of the line (rgba)
		 */
		[[deprecated("lineWidth is no longer a supported parameter, see overloads without lineWidth")]]
		static void DrawVector(float* origin, float* vectorToDraw, float lineWidth = 1.0f, float *colour = nullptr);
		/*
		 * Draws a vector, as a given point and direction
		 * @param origin The point to draw the vector from
		 * @param vectorToDraw The vector to draw
		 * @param colour The color of the line (rgba)
		 */
		static void DrawVector(float* origin, float* vectorToDraw, float* colour = nullptr);

		// Description:
		// Draws point at specified position.
		// Arguments:
		// p0 is an array of 3 floats representing position
		// pointSize lets you specify the size of the point, in pixels (p0 is center of point)
		static void DrawPoint(const glm::vec3& p0, float pointSize, const glm::vec4& colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		static void DrawPoint(float *p0, float pointSize, float *colour = nullptr);

		// Description:
		// Draws a cube at position p0 with the specified size
		// This will set the camera mode to model view, push matrix apply the
		// cube's transformation and pop matrix
		// Colour is expected to be an array of four floats (rgba)
		static void DrawCube(const glm::vec3& p0, float size, const glm::vec4& colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		static void DrawCube(float *p0, float size = 1.0f, float *colour = nullptr);
		
		//static void DrawPrism(TTK::Matrix4x4, TTK::Vector3 colour, float scale);

		// Description:
		// Draws teabro
		static void DrawTeapot(const glm::vec3& p0, float size, const glm::vec4 colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		static void DrawTeapot(float *p0, float size = 1.0f, float *colour = nullptr);

		/*
		 * Draws a sphere in the scene, with the given position, size, and color
		 * @param center The center of the sphere
		 * @param size The radius of the sphere
		 * @param colour The color to draw the sphere in
		 */
		static void DrawSphere(const glm::vec3& center, float size, const glm::vec4& colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		/*
		 * Draws a sphere in the scene, with the given transformation
		 * @param p0 The transformation to apply to the unit sphere (note that the radius of the sphere is 1 by default)
		 * @param size[deprecated] The radius of the sphere
		 * @param colour The color to draw the sphere in
		 */
		[[deprecated("size does not actually do anything here, please use the overload that accepts just a transform and a color")]]
		static void DrawSphere(const glm::mat4& p0, float size, const glm::vec4& colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		/*
		 * Draws a sphere in the scene, with the given transformation
		 * @param p0 The transformation to apply to the unit sphere (note that the radius of the sphere is 1 by default)
		 * @param colour The color to draw the sphere in
		 */
		static void DrawSphere(const glm::mat4& transform, const glm::vec4& colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		/*
		 * Draws a sphere in the scene, with the given position, size, and color
		 * @param p0 The center of the sphere, array of 3 floats
		 * @param size The radius of the sphere
		 * @param colour The color to draw the sphere in (array of 4 floats, rgba)
		 */
		static void DrawSphere(float *p0, float size = 1.0f, float *colour = nullptr);

		// Description:
		// Clears the current projection matrix and then resets it
		// to an orthographic projection.
		// Note: This function uses deprecated opengl functions.
		// See TTK::Camera for a modern implementation
		static void SetCameraMode2D(int windowWidth, int windowHeight);

		// Description:
		// Clears the current projection matrix and then resets it
		// to an perspective projection with a field of view (FOV) of 60 degrees.
		// Note: This function uses deprecated opengl functions.
		// See TTK::Camera for a modern implementation
		static void SetCameraMode3D(int windowWidth, int windowHeight, float fov = 60.0f);

		static void SetViewport(int x, int y, int w, int h);

		/*
		 * Enables or disables depth testing. When enabled, objects behind other objects will be occluded (not visible)
		 * @param isEnabled Whether or not depth testing is enabled
		 */
		static void SetDepthEnabled(bool isEnabled = true);

		/*
		 * Sets the view matrix for TTK to use when rendering, to not use a camera, call this with either no parameters,
		 * or the identity matrix
		 * @param view The camera's view matrix (default is identity matrix)
		 */
		static void SetCameraMatrix(const glm::mat4& view = glm::mat4(1.0f));

		/*
		 * Gets the view projection that TTK is currently using for rendering
		 */
		static const glm::mat4& GetViewProjection();

		// Description:
		// Sets the colour of the screen after ClearScreen() has been called.
		static void SetBackgroundColour(float r, float g, float b);

		// Description:
		// Removes all content from the screen. This should be called at
		// the beginning of every draw function.
		static void ClearScreen();

		/*
		 * Handles rendering any lines and triangles that have been added and not yet flushed to the screen
		 */
		static void EndFrame();

		/*
		 * Draws a grid with 21 lines centered around the origin on the xy plane (10 negative, 10 positive)
		 * @param gridWidth The size of the gap between line segments (default 10 units)
		 * @param mode The up mode for the grid, by default this assumes that Z is up
		 */
		static void DrawGrid(float gridWidth = 10.0f, AlignMode mode = AlignMode::YUp);

		/*
		 * Handles freeing the resources used by TTK
		 */
		static void Cleanup();
	};
}

#endif