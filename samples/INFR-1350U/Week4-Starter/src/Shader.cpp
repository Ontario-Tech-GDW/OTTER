#include "Shader.h"
#include "Logging.h"
#include <fstream>
#include <sstream>

Shader::Shader() :
	_vs(0),
	_fs(0),
	_handle(0)
{
	_handle = glCreateProgram();
}

Shader::~Shader() {
	if (_handle != 0) {
		glDeleteProgram(_handle);
		_handle = 0;
	}
}

bool Shader::LoadShaderPart(const char* source, GLenum type)
{
	// Creates a new shader part (VS, FS, GS, etc...)
	GLuint handle = glCreateShader(type);

	// Load the GLSL source and compile it
	glShaderSource(handle, 1, &source, nullptr);
	glCompileShader(handle);

	// Get the compilation status for the shader part
	GLint status = 0;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		// Get the size of the error log
		GLint logSize = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);

		// Create a new character buffer for the log
		char* log = new char[logSize];

		// Get the log
		glGetShaderInfoLog(handle, logSize, &logSize, log);

		// Dump error log
		LOG_ERROR("Failed to compile shader part:\n{}", log);

		// Clean up our log memory
		delete[] log;

		// Delete the broken shader result
		glDeleteShader(handle);
		handle = 0;
	}

	switch (type) {
		case GL_VERTEX_SHADER: _vs = handle; break;
		case GL_FRAGMENT_SHADER: _fs = handle; break;
		default: LOG_WARN("Not implemented"); break;
	}

	return status != GL_FALSE;
}

bool Shader::LoadShaderPartFromFile(const char* path, GLenum type) {
	std::ifstream file(path);
	if (!file.is_open()) {
		LOG_ERROR("File not found: {}", path);
		throw std::runtime_error("File not found, see logs for more information");
	}
	std::stringstream stream;
	stream << file.rdbuf();
	bool result = LoadShaderPart(stream.str().c_str(), type);
	file.close();
	return result;
}

bool Shader::Link()
{
	LOG_ASSERT(_vs != 0 && _fs != 0, "Must attach both a vertex and fragment shader!");

	// Attach our two shaders
	glAttachShader(_handle, _vs);
	glAttachShader(_handle, _fs);

	// Perform linking
	glLinkProgram(_handle);

	// Remove shader parts to save space (we can do this since we only needed the shader parts to compile an actual shader program)
	glDetachShader(_handle, _vs);
	glDeleteShader(_vs);
	glDetachShader(_handle, _fs);
	glDeleteShader(_fs);

	GLint status = 0;
	glGetProgramiv(_handle, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		// Get the length of the log
		GLint length = 0;
		glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			// Read the log from openGL
			char* log = new char[length];
			glGetProgramInfoLog(_handle, length, &length, log);
			LOG_ERROR("Shader failed to link:\n{}", log);
			delete[] log;
		}
		else {
			LOG_ERROR("Shader failed to link for an unknown reason!");
		}
	}
	return status != GL_FALSE;
}

void Shader::Bind() {
	glUseProgram(_handle);
}

void Shader::UnBind() {
	glUseProgram(0);
}
