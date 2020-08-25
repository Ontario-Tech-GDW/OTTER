//////////////////////////////////////////////////////////////////////////
//
// This file is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this file in your GDW games.
//
// This file implements the TTK batched rendering context
//
// Based off of TTK by Michael Gharbharan 2017
// Shawn Matthews 2019
//
//////////////////////////////////////////////////////////////////////////

#include "TTK/TTKContext.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <string>
#include "Logging.h"
#include "TTK/MeshHelper.h"

TTK::Context* TTK::Context::m_Instance = nullptr;

TTK::Context::~Context() {
	delete m_MeshHelper;
	delete m_DefaultFont;
	glDeleteBuffers(1, &m_Tris.VBO);
	glDeleteBuffers(1, &m_Lines.VBO);
	glDeleteBuffers(1, &m_Points.VBO);
	glDeleteVertexArrays(1, &m_Tris.VAO);
	glDeleteVertexArrays(1, &m_Lines.VAO);
	glDeleteVertexArrays(1, &m_Points.VAO);
	glDeleteProgram(m_ShaderHandle);
}

glm::mat4 TTK::Context::GetOrthoProjection() const {
	return glm::ortho(0.0f, static_cast<float>(m_WindowWidth), static_cast<float>(m_WindowHeight), 0.0f, -100.0f, 100.0f);
}

void TTK::Context::SetWindowSize(int windowWidth, int windowHeight) {
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;

	glViewport(0, 0, m_WindowWidth, m_WindowHeight);
}

void TTK::Context::RenderText(const char* text, const glm::vec2& position, const glm::vec4& color, float scale) {
	TTK::FontRenderer::Instance().Render(*m_DefaultFont, text, position, color, scale);
}

void TTK::Context::DrawTeapot(const glm::mat4& mat, const glm::vec4& color) const {
	m_MeshHelper->RenderTeapot(mat, color);
}

void TTK::Context::DrawSphere(const glm::mat4& mat, const glm::vec4& color) const {
	m_MeshHelper->RenderSphere(mat, color);
}

void TTK::Context::DrawCube(const glm::mat4& mat, const glm::vec4& color) const {
	m_MeshHelper->RenderCube(mat, color);
}

void TTK::Context::AddLine(const glm::vec3& a, const glm::vec3& b, const glm::vec4& color) {
	m_LineVerts[m_Lines.Count + 0].Position = a;
	m_LineVerts[m_Lines.Count + 0].Color = color;
	m_LineVerts[m_Lines.Count + 1].Position = b;
	m_LineVerts[m_Lines.Count + 1].Color = color;
	m_Lines.Count += 2;
	if (m_Lines.Count == MaxLineVerts) {
		__Flush(m_Lines);
	}
}

void TTK::Context::AddTri(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec4& color) {
	m_TriVerts[m_Tris.Count + 0].Position = a;
	m_TriVerts[m_Tris.Count + 0].Color = color;
	m_TriVerts[m_Tris.Count + 1].Position = b;
	m_TriVerts[m_Tris.Count + 1].Color = color;
	m_TriVerts[m_Tris.Count + 2].Position = c;
	m_TriVerts[m_Tris.Count + 2].Color = color;
	m_Tris.Count += 3;
	if (m_Tris.Count == MaxTriVerts) {
		__Flush(m_Tris);
	}
}

void TTK::Context::AddQuad(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color) {
	glm::vec3 minXmaxY = { min.x, max.y, min.z };
	glm::vec3 maxXminY = { max.x, min.y, min.z };
	AddTri(min, maxXminY, minXmaxY, color);
	AddTri(maxXminY, max, minXmaxY, color);
}

void TTK::Context::AddPoint(const glm::vec3& pos, float size, const glm::vec4& color)
{
	m_PointVerts[m_Points.Count].Position = pos;
	m_PointVerts[m_Points.Count].Color = color;
	m_PointVerts[m_Points.Count].Size = size;
	m_Points.Count += 1;
	if (m_Points.Count == MaxPointVerts) {
		__Flush(m_Points);
	}
}

void TTK::Context::Flush() {
	__Flush(m_Tris);
	__Flush(m_Lines);
	__Flush(m_Points);
}

TTK::Context::Context() {
	m_Projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	m_ViewMatrix = glm::mat4(1.0f);
	m_DefaultFont = new TrueTypeTextureFont("C:\\\\Windows\\Fonts\\consola.ttf", 32);
	
	const char* vsSource = R"LIT(#version 430
            layout (location = 0) uniform mat4 xTransform;
	
            layout (location = 0) in vec3 vertexPosition;
            layout (location = 1) in vec4 vertexColor;
	
            layout (location = 0) out vec4 fragmentColor;
            void main() {
                gl_Position = xTransform * vec4(vertexPosition, 1);
                fragmentColor = vertexColor;
            })LIT";

	const char* fsSource = R"LIT(#version 430
            layout (location = 0) in vec4 fragColor;     	
            out vec4 frag_color;            	
            void main() {
                frag_color = fragColor;
            })LIT";

	m_ShaderHandle = __CompileShader(vsSource, fsSource);

	const char* vsSourcePoint = R"LIT(#version 430
            layout (location = 0) uniform mat4 xTransform;
	
            layout (location = 0) in vec3 vertexPosition;
            layout (location = 1) in vec4 vertexColor;
            layout (location = 2) in float vertexPointSize;
	
            layout (location = 0) out vec4 fragmentColor;
            void main() {
                gl_Position = xTransform * vec4(vertexPosition, 1);
				gl_PointSize = vertexPointSize;
                fragmentColor = vertexColor;
            })LIT";
	
	m_PointShaderHandle = __CompileShader(vsSourcePoint, fsSource);


	m_Tris = __InitBuff(GL_TRIANGLES, m_ShaderHandle, m_TriVerts, sizeof(SimpleVert), MaxTriVerts);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(SimpleVert), (void*)offsetof(SimpleVert, Position));
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(SimpleVert), (void*)offsetof(SimpleVert, Color));

	m_Lines = __InitBuff(GL_LINES, m_ShaderHandle, m_LineVerts, sizeof(SimpleVert), MaxLineVerts);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(SimpleVert), (void*)offsetof(SimpleVert, Position));
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(SimpleVert), (void*)offsetof(SimpleVert, Color));

	m_Points = __InitBuff(GL_POINTS, m_PointShaderHandle, m_PointVerts, sizeof(PointVert), MaxLineVerts);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(PointVert), (void*)offsetof(PointVert, Position));
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(PointVert), (void*)offsetof(PointVert, Color));
	glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(PointVert), (void*)offsetof(PointVert, Size));

	glBindVertexArray(0);

	// Make sure that the mesh helper has a context
	m_MeshHelper = new Impl::MeshHelper();

	// Allow our shaders to specify a point size
	glEnable(GL_PROGRAM_POINT_SIZE);
}

TTK::Context::GLBuff TTK::Context::__InitBuff(GLenum mode, GLuint shader, void* dataSource, size_t elemSize, size_t maxElems)
{
	GLBuff result;
	result.Mode = mode;
	result.Count = 0;
	result.Data = dataSource;
	result.ElemSize = elemSize;
	result.Shader = shader;

	glCreateVertexArrays(1, &result.VAO);
	glBindVertexArray(result.VAO);
	glCreateBuffers(1, &result.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, result.VBO);
	glNamedBufferData(result.VBO, elemSize * maxElems, nullptr, GL_STREAM_DRAW);

	return result;
}

void TTK::Context::__Flush(GLBuff& buff) {
	if (buff.Count > 0) {
		glUseProgram(buff.Shader);
		glUniformMatrix4fv(0, 1, false, &m_ViewProjection[0][0]);
		glNamedBufferSubData(buff.VBO, 0, buff.Count * buff.ElemSize, buff.Data);
		glBindVertexArray(buff.VAO);
		glDrawArrays(buff.Mode, 0, static_cast<GLsizei>(buff.Count));
		buff.Count = 0;
	}
}

GLuint TTK::Context::__CompileShader(const char* vsSource, const char* fsSource)
{
	GLuint result = glCreateProgram();

	GLuint programs[2];
	programs[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(programs[0], 1, &vsSource, NULL);
	glCompileShader(programs[0]);
	programs[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(programs[1], 1, &fsSource, NULL);
	glCompileShader(programs[1]);

	// Attach our two shaders
	glAttachShader(result, programs[0]);
	glAttachShader(result, programs[1]);

	// Perform linking
	glLinkProgram(result);

	GLint success = 0;
	glGetProgramiv(result, GL_LINK_STATUS, &success);
	LOG_INFO("Status: {}", success);

	if (success == GL_FALSE) {
		// Get the length of the log
		GLint length = 0;
		glGetProgramiv(result, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			// Read the log from openGL
			char* log = new char[length];
			glGetProgramInfoLog(result, length, &length, log);
			LOG_ERROR("Shader failed to link:\n{}", log);
			delete[] log;
		}
		else {
			LOG_ERROR("Shader failed to link for an unknown reason!");
		}

		// Delete the partial program
		glDeleteProgram(result);

		// Throw a runtime exception
		throw new std::runtime_error("Failed to link shader program!");
	}

	// Remove shader parts to save space
	glDetachShader(result, programs[0]);
	glDeleteShader(programs[0]);
	glDetachShader(result, programs[1]);
	glDeleteShader(programs[1]);

	return result;
}
