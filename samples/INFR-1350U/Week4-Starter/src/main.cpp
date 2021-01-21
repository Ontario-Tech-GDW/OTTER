#include <Logging.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <json.hpp>
#include <fstream>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArrayObject.h"
#include "Shader.h"
#include "Camera.h"

#define LOG_GL_NOTIFICATIONS

/*
	Handles debug messages from OpenGL
	https://www.khronos.org/opengl/wiki/Debug_Output#Message_Components
	@param source    Which part of OpenGL dispatched the message
	@param type      The type of message (ex: error, performance issues, deprecated behavior)
	@param id        The ID of the error or message (to distinguish between different types of errors, like nullref or index out of range)
	@param severity  The severity of the message (from High to Notification)
	@param length    The length of the message
	@param message   The human readable message from OpenGL
	@param userParam The pointer we set with glDebugMessageCallback (should be the game pointer)
*/
void GlDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::string sourceTxt;
	switch (source) {
	case GL_DEBUG_SOURCE_API: sourceTxt = "DEBUG"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceTxt = "WINDOW"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceTxt = "SHADER"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: sourceTxt = "THIRD PARTY"; break;
	case GL_DEBUG_SOURCE_APPLICATION: sourceTxt = "APP"; break;
	case GL_DEBUG_SOURCE_OTHER: default: sourceTxt = "OTHER"; break;
	}
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:          LOG_INFO("[{}] {}", sourceTxt, message); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG_WARN("[{}] {}", sourceTxt, message); break;
	case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR("[{}] {}", sourceTxt, message); break;
		#ifdef LOG_GL_NOTIFICATIONS
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INFO("[{}] {}", sourceTxt, message); break;
		#endif
	default: break;
	}
}

GLFWwindow* window;

void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

bool InitGLFW() {
	if (glfwInit() == GLFW_FALSE) {
		LOG_ERROR("Failed to initialize GLFW");
		return false;
	}

	//Create a new GLFW window
	window = glfwCreateWindow(800, 800, "INFR1350U", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set our window resized callback
	glfwSetWindowSizeCallback(window, GlfwWindowResizedCallback);

	return true;
}

bool InitGLAD() {
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		LOG_ERROR("Failed to initialize Glad");
		return false;
	}
}

int main() {
	Logger::Init(); // We'll borrow the logger from the toolkit, but we need to initialize it

	//Initialize GLFW
	if (!InitGLFW())
		return 1;

	//Initialize GLAD
	if (!InitGLAD())
		return 1;

	// Let OpenGL know that we want debug output, and route it to our handler function
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GlDebugMessage, nullptr);

	static const float points[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	static const float colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	//VBO - Vertex buffer object
	VertexBuffer* posVbo = new VertexBuffer();
	posVbo->LoadData(points, 9);

	VertexBuffer* color_vbo = new VertexBuffer();
	color_vbo->LoadData(colors, 9);

	VertexArrayObject* vao = new VertexArrayObject();
	vao->AddVertexBuffer(posVbo, {
		BufferAttribute(0, 3, GL_FLOAT, false, 0, NULL)
	});
	vao->AddVertexBuffer(color_vbo, {
		BufferAttribute(1, 3, GL_FLOAT, false, 0, NULL)
	});

	static const float interleaved[] = {
    //     X      Y     Z       R     G    B
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  0.3f, 0.2f, 0.5f,
	    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,
		 0.5f,  1.0f, 0.0f,  1.0f, 1.0f, 1.0f
	};

	VertexBuffer* interleaved_vbo = new VertexBuffer();
	interleaved_vbo->LoadData(interleaved, 6 * 4);

	static const uint16_t indices[] = {
		0, 1, 2,
		1, 3, 2
	};
	IndexBuffer* interleaved_ibo = new IndexBuffer();
	interleaved_ibo->LoadData(indices, 3 * 2);

	size_t stride = sizeof(float) * 6;
	VertexArrayObject* vao2 = new VertexArrayObject();
	vao2->AddVertexBuffer(interleaved_vbo, {
		BufferAttribute(0, 3, GL_FLOAT, false, stride, 0),
		BufferAttribute(1, 3, GL_FLOAT, false, stride, sizeof(float) * 3),
	});
	vao2->SetIndexBuffer(interleaved_ibo);

	// Load our shaders
	Shader* shader = new Shader();
	shader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	shader->LoadShaderPartFromFile("shaders/frag_shader.glsl", GL_FRAGMENT_SHADER);
	shader->Link();

	// GL states
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLint xTransformLoc = glGetUniformLocation(shader->GetHandle(), "u_ModelViewProjection");
	glm::mat4 transform = glm::mat4(1.0f);

	// Our high-precision timer
	double lastFrame = glfwGetTime();

	///// Game loop /////
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Calculate the time since our last frame (dt)
		double thisFrame = glfwGetTime();
		float dt = static_cast<float>(thisFrame - lastFrame);
		transform = glm::rotate(glm::mat4(1.0f), static_cast<float>(thisFrame), glm::vec3(0, 0, 1));

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Bind();
		glProgramUniformMatrix4fv(shader->GetHandle(), xTransformLoc, 1, false, glm::value_ptr(transform));

		vao->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		vao->UnBind();

		vao2->Bind();
		glDrawElements(GL_TRIANGLES, interleaved_ibo->GetElementCount(), interleaved_ibo->GetElementType(), nullptr);
		vao->UnBind();

		glfwSwapBuffers(window);
	}

	delete shader;
	delete vao;
	delete vao2;
	delete interleaved_vbo;
	delete interleaved_ibo;
	delete posVbo;
	delete color_vbo;

	// Clean up the toolkit logger so we don't leak memory
	Logger::Uninitialize();
	return 0;
}