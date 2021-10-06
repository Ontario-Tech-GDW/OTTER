#include "Shader.h"
#include "Logging.h"
#include <fstream>
#include <sstream>

Shader::Shader() :
	// We zero out all of our members so we don't have garbage data in our class
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

bool Shader::LoadShaderPart(const char* source, ShaderPartType type)
{
	// Creates a new shader part (VS, FS, GS, etc...)
	GLuint handle = glCreateShader((GLenum)type);

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

		return false;
	}

	switch (type) {
		case ShaderPartType::Vertex: _vs = handle; break;
		case ShaderPartType::Fragment: _fs = handle; break;
		default: LOG_WARN("Not implemented"); break;
	}

	return status != GL_FALSE;
}

bool Shader::LoadShaderPartFromFile(const char* path, ShaderPartType type) {
	// Open the file at path
	std::ifstream file(path);

	// Check to see if the file is open
	if (file.is_open()) {
		// Create a string stream to load the entire contents of the file
		std::stringstream stream;
		stream << file.rdbuf();

		// Compile the shader part from the loaded contents of the file
		bool result = LoadShaderPart(stream.str().c_str(), type);

		// Close the file
		file.close();
		return result;
	}
	// Failed to open file, log it and return false
	else {
		LOG_WARN("Could not open file at \"{}\"", path);
		return false;
	}
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
		} else {
			LOG_ERROR("Shader failed to link for an unknown reason!");
		}
	}
	return status != GL_FALSE;
}

void Shader::Bind() {
	// Simply calls glUseProgram with our shader handle
	glUseProgram(_handle);
}

void Shader::Unbind() {
	// We unbind a shader program by using the default program (0)
	glUseProgram(0);
}
