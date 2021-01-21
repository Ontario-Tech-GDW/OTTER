#include "SimpleMoveBehaviour.h"
#include "Application.h"
#include "Timing.h"
#include "Transform.h"

#include "GLFW/glfw3.h"

void SimpleMoveBehaviour::Update(entt::handle entity)
{
	float dt = Timing::Instance().DeltaTime;
	GLFWwindow* window = Application::Instance().Window;
	Transform& transform = entity.get<Transform>();

	if (Relative) {
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			transform.MoveLocal(0.0f, -1.0f * dt, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			transform.MoveLocal(0.0f, 1.0f * dt, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			transform.MoveLocal(-1.0f * dt, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			transform.MoveLocal(1.0f * dt, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			transform.MoveLocal(0.0f, 0.0f, 1.0f * dt);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			transform.MoveLocal(0.0f, 0.0f, -1.0f * dt);
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			transform.RotateLocal(0.0f, -45.0f * dt, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			transform.RotateLocal(0.0f, 45.0f * dt, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			transform.RotateLocal(45.0f * dt, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			transform.RotateLocal(-45.0f * dt, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			transform.RotateLocal(0.0f, 0.0f, 45.0f * dt);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			transform.RotateLocal(0.0f, 0.0f, -45.0f * dt);
		}
	} else
	{
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			transform.MoveLocalFixed(0.0f, -1.0f * dt, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			transform.MoveLocalFixed(0.0f, 1.0f * dt, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			transform.MoveLocalFixed(-1.0f * dt, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			transform.MoveLocalFixed(1.0f * dt, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			transform.MoveLocalFixed(0.0f, 0.0f, 1.0f * dt);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			transform.MoveLocalFixed(0.0f, 0.0f, -1.0f * dt);
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			transform.RotateLocalFixed(0.0f, -45.0f * dt, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			transform.RotateLocalFixed(0.0f, 45.0f * dt, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			transform.RotateLocalFixed(45.0f * dt, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			transform.RotateLocalFixed(-45.0f * dt, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			transform.RotateLocalFixed(0.0f, 0.0f, 45.0f * dt);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			transform.RotateLocalFixed(0.0f, 0.0f, -45.0f * dt);
		}
	}
}
