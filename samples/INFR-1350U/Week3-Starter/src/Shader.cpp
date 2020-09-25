#include "Shader.h"
#include "Logging.h"
#include <fstream>
#include <sstream>

Shader::Shader() :
	_vs(0),
	_fs(0),
	_handle(0)
{
	// TODO: Constructor
}

Shader::~Shader() {
	// TODO: Destructor
}

bool Shader::LoadShaderPart(const char* source, GLenum type)
{
	// TODO: Implement
}

bool Shader::LoadShaderPartFromFile(const char* path, GLenum type) {
	std::ifstream file(path);
	std::stringstream stream;
	stream << file.rdbuf();
	bool result = LoadShaderPart(stream.str().c_str(), type);
	file.close();
	return result;
}

bool Shader::Link()
{
	// Todo: Implement
}

void Shader::Bind() {
	glUseProgram(_handle);
}

void Shader::UnBind() {
	glUseProgram(0);
}
