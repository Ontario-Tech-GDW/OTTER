//////////////////////////////////////////////////////////////////////////
//
// This file is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this file in your GDW games.
//
// This file implements the Mesh Helper for TTK
//
// Based off of TTK by Michael Gharbharan 2017
// Shawn Matthews 2019
//
//////////////////////////////////////////////////////////////////////////
#include "TTK/MeshHelper.h"

#include "TTK/Teapot.h"
#include "TTK/Sphere.h"
#include "TTK/Cube.h"
#include "Logging.h"


TTK::Impl::MeshHelper::~MeshHelper() {
	glDeleteBuffers(1, &m_Teapot.VBO);
	glDeleteBuffers(1, &m_Sphere.VBO);
	glDeleteBuffers(1, &m_Cube.VBO);
	glDeleteVertexArrays(1, &m_Teapot.VAO);
	glDeleteVertexArrays(1, &m_Sphere.VAO);
	glDeleteVertexArrays(1, &m_Cube.VAO);
	glDeleteProgram(m_Shader);
}

void TTK::Impl::MeshHelper::RenderTeapot(const glm::mat4& transform, const glm::vec4& color) const {
	glUseProgram(m_Shader);
	glm::mat4 t = Context::Instance().GetViewProjection() * transform;
	glProgramUniformMatrix4fv(m_Shader, 0, 1, FALSE, &t[0][0]);
	glProgramUniform4fv(m_Shader, 1, 1, &color[0]);
	glBindVertexArray(m_Teapot.VAO);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(TeapotData) / (sizeof(float) * 6));
}

void TTK::Impl::MeshHelper::RenderSphere(const glm::mat4& transform, const glm::vec4& color) const {
	glUseProgram(m_Shader);
	glm::mat4 t = Context::Instance().GetViewProjection() * transform;
	glProgramUniformMatrix4fv(m_Shader, 0, 1, FALSE, &t[0][0]);
	glProgramUniform4fv(m_Shader, 1, 1, &color[0]);
	glBindVertexArray(m_Sphere.VAO);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(SphereData) / (sizeof(float) * 6));
}

void TTK::Impl::MeshHelper::RenderCube(const glm::mat4& transform, const glm::vec4& color) const
{
	glUseProgram(m_Shader);
	glm::mat4 t = Context::Instance().GetViewProjection() * transform;
	glProgramUniformMatrix4fv(m_Shader, 0, 1, FALSE, &t[0][0]);
	glProgramUniform4fv(m_Shader, 1, 1, &color[0]);
	glBindVertexArray(m_Cube.VAO);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(CubeData) / (sizeof(float) * 6));
}

TTK::Impl::MeshHelper::mesh TTK::Impl::MeshHelper::__MakeMesh(const float* data, size_t size) const {
	mesh result;
	glCreateVertexArrays(1, &result.VAO);
	glBindVertexArray(result.VAO);
	glCreateBuffers(1, &result.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, result.VBO);
	glNamedBufferData(result.VBO, size, data, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 6, 0);
	return result;
}

TTK::Impl::MeshHelper::MeshHelper()
{
	m_Teapot = __MakeMesh(TeapotData, sizeof(TeapotData));
	m_Sphere = __MakeMesh(SphereData, sizeof(SphereData));
	m_Cube   = __MakeMesh(CubeData, sizeof(CubeData));
	
	glBindVertexArray(0);
	
	const char* vsSource = R"LIT(#version 430
            layout (location = 0) in vec3 vertexPosition;
            layout (location = 0) uniform mat4 xTransform;	
            void main() {
                gl_Position = xTransform * vec4(vertexPosition, 1);
            })LIT";

	const char* fsSource = R"LIT(#version 430   
            layout (location = 1) uniform vec4 xColor;		
            out vec4 frag_color;            	
            void main() {
                frag_color = xColor;
            })LIT";

	m_Shader = glCreateProgram();

	GLuint programs[2];
	programs[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(programs[0], 1, &vsSource, NULL);
	glCompileShader(programs[0]);
	programs[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(programs[1], 1, &fsSource, NULL);
	glCompileShader(programs[1]);

	// Attach our two shaders
	glAttachShader(m_Shader, programs[0]);
	glAttachShader(m_Shader, programs[1]);

	// Perform linking
	glLinkProgram(m_Shader);

	GLint success = 0;
	glGetProgramiv(m_Shader, GL_LINK_STATUS, &success);
	LOG_INFO("Status: {}", success);

	if (success == GL_FALSE) {
		// Get the length of the log
		GLint length = 0;
		glGetProgramiv(m_Shader, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			// Read the log from openGL
			char* log = new char[length];
			glGetProgramInfoLog(m_Shader, length, &length, log);
			LOG_ERROR("Shader failed to link:\n{}", log);
			delete[] log;
		}
		else {
			LOG_ERROR("Shader failed to link for an unknown reason!");
		}

		// Delete the partial program
		glDeleteProgram(m_Shader);

		// Throw a runtime exception
		throw new std::runtime_error("Failed to link shader program!");
	}

	// Remove shader parts to save space
	glDetachShader(m_Shader, programs[0]);
	glDeleteShader(programs[0]);
	glDetachShader(m_Shader, programs[1]);
	glDeleteShader(programs[1]);
}
