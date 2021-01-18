//////////////////////////////////////////////////////////////////////////
//
// This file is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this file in your GDW games.
//
// This file contains the implementations for the exposed GraphicsUtils
// API of TTK
//
// Based off of TTK by Michael Gharbharan 2017
// Shawn Matthews 2019
//
//////////////////////////////////////////////////////////////////////////

#include "TTK/GraphicsUtils.h"
#include "TTK/TTKContext.h"
#include <GLM/gtc/matrix_transform.inl>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

// Helpers to convert raw pointers to glm types, and some colors
#define BLACK glm::vec4(0, 0, 0, 1)
#define WHITE glm::vec4(1, 1, 1, 1)
#define VEC3(ptr) *(glm::vec3*)ptr
#define VEC4(ptr) *(glm::vec4*)ptr
#define DEFAULT_BLACK(ptr) ptr == nullptr ? BLACK : VEC4(ptr)
#define DEFAULT_WHITE(ptr) ptr == nullptr ? WHITE : VEC4(ptr)

void TTK::Graphics::SetCameraMode2D(int windowWidth, int windowHeight) {
	TTK::Context::Instance().SetWindowSize(windowWidth, windowHeight);
	TTK::Context::Instance().SetProjection(glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, -100.0f, 100.0f));
}

void TTK::Graphics::SetCameraMode3D(int windowWidth, int windowHeight, float fov) {
	TTK::Context::Instance().SetWindowSize(windowWidth, windowHeight);
	if (windowWidth > 0 && windowHeight > 0) {
		TTK::Context::Instance().SetProjection(glm::perspective(glm::radians(fov), (float)windowWidth / (float)windowHeight, 0.001f, 1000.0f));
	}
}

void TTK::Graphics::SetViewport(int x, int y, int w, int h)
{
	TTK::Context::Instance().SetViewport(x, y, w, h);
}

void TTK::Graphics::SetDepthEnabled(bool isEnabled) {
	if (isEnabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void TTK::Graphics::SetCameraMatrix(const glm::mat4& view) {
	TTK::Context::Instance().SetView(view);
}

const glm::mat4& TTK::Graphics::GetViewProjection() {
	return TTK::Context::Instance().GetViewProjection();
}

void TTK::Graphics::SetBackgroundColour(float r, float g, float b) {
	glClearColor(r, g, b, 1.0f);
}

void TTK::Graphics::ClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void TTK::Graphics::EndFrame() {
	TTK::Context::Instance().Flush();
}

void TTK::Graphics::DrawGrid(float gridWidth, AlignMode mode) {
	// Note: since we want to support both Y-Up and Z-Up, our points are a little
	// more complex. We set the y in z-up mode, or z in y-up mode. We use ternary
	// operators to keep the code from getting large
	const float gridMin{ -10.0f * gridWidth }, gridMax{ 10.0f * gridWidth };
	for(int x = -10; x <= 10; x++) {
		TTK::Context::Instance().AddLine(
			{ x * gridWidth, mode == TTK::AlignMode::ZUp ? gridMin : 0, mode == AlignMode::YUp ? gridMin : 0 },
			{ x * gridWidth, mode == AlignMode::ZUp ? gridMax : 0, mode == AlignMode::YUp ? gridMax : 0 },
			x == 0 ? glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		for(int y = -10; y <= 10; y++) {
			TTK::Context::Instance().AddLine(
				{ gridMin, mode == AlignMode::ZUp ? y * gridWidth : 0, mode == AlignMode::YUp ? y * gridWidth : 0 },
				{ gridMax, mode == AlignMode::ZUp ? y * gridWidth : 0, mode == AlignMode::YUp ? y * gridWidth : 0 },
				y == 0 ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}
}

void TTK::Graphics::Cleanup() {
	TTK::Context::DestroyContext();
	TTK::FontRenderer::DestroyContext();
}

void TTK::Graphics::DrawText2D(const std::string& text, float posX, float posY, float fontSize) {
	DrawText2D(text, posX, posY, { 0, 0, 0, 1 }, fontSize);
}

void TTK::Graphics::DrawText2D(const std::string& text, float posX, float posY, const glm::vec4& color, float fontSize) {
	TTK::Context::Instance().RenderText(text.c_str(), { posX, posY }, color, fontSize / 32.0f);
}

void TTK::Graphics::InitImGUI(GLFWwindow* window) {
	// Creates a new ImGUI context5
	ImGui::CreateContext();
	// Gets our ImGUI input/output 
	ImGuiIO& io = ImGui::GetIO();
	// Enable keyboard navigation
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Allow docking to our window
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Allow multiple viewports (so we can drag ImGui off our window)
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// Allow our viewports to use transparent backbuffers
	io.ConfigFlags |= ImGuiConfigFlags_TransparentBackbuffers;

	// Set up the ImGui implementation for OpenGL
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	// Dark mode FTW
	ImGui::StyleColorsDark();

	// Get our imgui style
	ImGuiStyle& style = ImGui::GetStyle();
	//style.Alpha = 1.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.8f;
	}
}

void TTK::Graphics::ShutdownImGUI() {
	// Cleanup the ImGui implementation
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	// Destroy our ImGui context
	ImGui::DestroyContext();
}

void TTK::Graphics::BeginGUI() {
	// Implementation new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	// ImGui context new frame
	ImGui::NewFrame();
}

void TTK::Graphics::EndGUI() {
	// Cache the active window
	GLFWwindow* window = glfwGetCurrentContext();
	
	// Make sure ImGui knows how big our window is
	ImGuiIO& io = ImGui::GetIO();
	int width{ 0 }, height{ 0 };
	glfwGetWindowSize(window, &width, &height);
	io.DisplaySize = ImVec2(width, height);

	// Render all of our ImGui elements
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// If we have multiple viewports enabled (can drag into a new window)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		// Update the windows that ImGui is using
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		// Restore our gl context
		glfwMakeContextCurrent(window);
	}
}

void TTK::Graphics::DrawLine(const glm::vec3& p0, const glm::vec3& p1, float lineWidth, const glm::vec4& colour) {
	TTK::Context::Instance().AddLine(p0, p1, colour);
}

void TTK::Graphics::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& colour) {
	TTK::Context::Instance().AddLine(p0, p1, colour);
}

void TTK::Graphics::DrawLine(float* p0, float* p1, float lineWidth, float* colour) {
	TTK::Context::Instance().AddLine(VEC3(p0), VEC3(p1), DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawLine(float* p0, float* p1, float* colour) {
	TTK::Context::Instance().AddLine(VEC3(p0), VEC3(p1), DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawVector(const glm::vec3& origin, const glm::vec3& vectorToDraw, float lineWidth, const glm::vec4& colour) {
	TTK::Context::Instance().AddLine(origin, origin + glm::normalize(vectorToDraw), colour);
}

void TTK::Graphics::DrawVector(const glm::vec3& origin, const glm::vec3& vectorToDraw, const glm::vec4& colour) {
	TTK::Context::Instance().AddLine(origin, origin + glm::normalize(vectorToDraw), colour);
}

void TTK::Graphics::DrawVector(float* p0, float* v0, float lineWidth, float* colour) {
	DrawVector(VEC3(p0), VEC3(v0), lineWidth, DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawVector(float* origin, float* vectorToDraw, float* colour) {
	DrawVector(VEC3(origin), VEC3(vectorToDraw), DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawPoint(const glm::vec3& p0, float pointSize, const glm::vec4& colour) {
	TTK::Context::Instance().AddPoint(p0, pointSize, colour);
}

void TTK::Graphics::DrawPoint(float* p0, float pointSize, float* colour) {
	DrawPoint(VEC3(p0), pointSize, DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawCube(const glm::vec3& p0, float size, const glm::vec4& colour) {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), p0) * glm::scale(glm::mat4(1.0f), glm::vec3(size));
	TTK::Context::Instance().DrawCube(transform, colour);
}

void TTK::Graphics::DrawCube(float* p0, float size, float* colour) {
	DrawCube(VEC3(p0), size, DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawTeapot(const glm::vec3& p0, float size, const glm::vec4 colour)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), p0) * glm::scale(glm::mat4(1.0f), glm::vec3(size));
	TTK::Context::Instance().DrawTeapot(transform, colour);
}

void TTK::Graphics::DrawTeapot(float* p0, float size, float* colour)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), *(glm::vec3*)p0) * glm::scale(glm::mat4(1.0f), glm::vec3(size));
	TTK::Context::Instance().DrawTeapot(transform, DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawSphere(const glm::vec3& center, float size, const glm::vec4& colour) {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), center) * glm::scale(glm::mat4(1.0f), glm::vec3(size));
	TTK::Context::Instance().DrawSphere(transform, colour);
}

void TTK::Graphics::DrawSphere(const glm::mat4& p0, float size, const glm::vec4& colour) {
	TTK::Context::Instance().DrawSphere(p0, colour);
}

void TTK::Graphics::DrawSphere(const glm::mat4& transform, const glm::vec4& colour) {
	TTK::Context::Instance().DrawSphere(transform, colour);
}

void TTK::Graphics::DrawSphere(float* p0, float size, float* colour) {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), *(glm::vec3*)p0) * glm::scale(glm::mat4(1.0f), glm::vec3(size));
	TTK::Context::Instance().DrawSphere(transform, DEFAULT_BLACK(colour));
}
