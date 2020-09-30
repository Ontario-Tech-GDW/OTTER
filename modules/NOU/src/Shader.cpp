/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Shader.cpp
Utility classes for managing OpenGL shaders and shader programs.
*/

#include "NOU/Shader.h"

#include "GLM/glm.hpp"

#include <iostream>
#include <fstream>

namespace nou
{
	const ShaderProgram* ShaderProgram::m_current = nullptr;

	//This loads in the file specifed as an array of GL characters.
	//This function allocates memory - it is the responsibility of the caller
	//to clean up the data pointer after they have used it!
	bool LoadFileGLChar(const std::string& path, GLchar*& dataOut, GLint& lenOut)
	{
		std::ifstream reader(path, std::ios::in);

		if (reader && reader.good())
		{
			std::string data = std::string(std::istreambuf_iterator<char>(reader),
				std::istreambuf_iterator<char>());

			lenOut = (GLint)data.length() + 1;

			dataOut = new char[data.length() + 1];
			std::copy(data.begin(), data.end(), dataOut);
			dataOut[data.length()] = '\0';

			reader.close();
			return true;
		}

		printf("File %s not found.\n", path.c_str());

		reader.close();
		return false;
	}

	//This will format and print an info log given by OpenGL in the event of issues
	//compiling shaders and linking shader programs.
	void PrintGLInfoLog(const std::string& preamble, GLInfoLogType logType, GLuint objID, GLint buflen)
	{
		std::string err = "Unknown error";

		GLsizei strlen = 0;

		if (buflen > 1)
		{
			GLchar* glErr = new GLchar[buflen];

			switch (logType)
			{
			case GLInfoLogType::SHADER:
				glGetShaderInfoLog(objID, buflen, &strlen, glErr);
				break;
			case GLInfoLogType::PROGRAM:
				glGetProgramInfoLog(objID, buflen, &strlen, glErr);
				break;
			}

			err = std::string((char*)(glErr));

			delete[] glErr;
		}

		printf("%s: %s\n", preamble.c_str(), err.c_str());
	}

	Shader::Shader(const std::string& file, GLenum shaderType)
	{
		GLchar* data = nullptr;
		GLint len = 0;

		m_id = 0;

		printf("Loading shader: %s\n", file.c_str());

		if (LoadFileGLChar(file, data, len))
		{
			//Create a new shader object in OpenGL.
			m_id = glCreateShader(shaderType);

			const GLint* glLen = (const GLint*)(&len);
			const GLchar** glData = (const GLchar**)(&data);

			//Specify the source code (read from our file by LoadFileGLChar)
			//and ask OpenGL to compile our shader.
			glShaderSource(m_id, 1, glData, glLen);
			glCompileShader(m_id);

			//Check for any issues.
			GLint result;
			glGetShaderiv(m_id, GL_COMPILE_STATUS, &result);

			//Print some feedback on shader compilation.
			if (result)
				printf("Shader compiled successfully.\n");
			else
			{
				GLint buflen = 0;

				glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &buflen);
				PrintGLInfoLog("Shader compilation failed", GLInfoLogType::SHADER, m_id, buflen);

				glDeleteShader(m_id);
			}
		}

		delete[] data;
	}

	Shader::~Shader()
	{
		glDeleteShader(m_id);
	}

	GLuint Shader::GetID()
	{
		return m_id;
	}

	ShaderProgram::ShaderProgram(const std::vector<Shader*>& shaders)
	{
		//Create a new shader program object.
		m_id = glCreateProgram();

		//Attach our shadders to the new program.
		for (auto* shader : shaders)
		{
			glAttachShader(m_id, shader->GetID());
		}

		//Attempt to link the new program.
		glLinkProgram(m_id);

		//Find out if linking was successful.
		GLint result;
		glGetProgramiv(m_id, GL_LINK_STATUS, &result);

		//Provide feedback on the program's linking.
		if (result)
			printf("Linked shader program successfully.\n");
		else
		{
			GLint buflen = 0;

			glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &buflen);
			PrintGLInfoLog("Shader program linking failed", GLInfoLogType::PROGRAM, m_id, buflen);
		}

		//Detach shaders from the program (once it is linked, the shaders
		//no longer need to be attached - this will let OpenGL clean up the 
		//memory properly when those shaders are later deleted).
		for (auto* shader : shaders)
		{
			glDetachShader(m_id, shader->GetID());
		}
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_id);
	}

	void ShaderProgram::Bind() const
	{
		glUseProgram(m_id);
		m_current = this;
	}

	const ShaderProgram* ShaderProgram::Current()
	{
		return m_current;
	}

	GLint ShaderProgram::GetUniformLoc(const std::string& name) const
	{
		return glGetUniformLocation(m_id, name.c_str());
	}

	template<>
	void ShaderProgram::SetUniform<int>(const std::string& name, const int& value) const
	{
		glUniform1i(GetUniformLoc(name), value);
	}

	template<>
	void ShaderProgram::SetUniform<float>(const std::string& name, const float& value) const
	{
		glUniform1f(GetUniformLoc(name), value);
	}

	template<>
	void ShaderProgram::SetUniform<glm::mat4>(const std::string& name, const glm::mat4& value) const
	{
		glUniformMatrix4fv(GetUniformLoc(name), 1, GL_FALSE, &value[0][0]);
	}

	template<>
	void ShaderProgram::SetUniform<glm::mat3>(const std::string& name, const glm::mat3& value) const
	{
		glUniformMatrix3fv(GetUniformLoc(name), 1, GL_FALSE, &value[0][0]);
	}

	template<>
	void ShaderProgram::SetUniform<glm::vec4>(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(GetUniformLoc(name), 1, &(value.x));
	}

	template<>
	void ShaderProgram::SetUniform<glm::vec3>(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(GetUniformLoc(name), 1, &(value.x));
	}

	template<>
	void ShaderProgram::SetUniformArray<glm::mat4>(const std::string& name, glm::mat4* data, int len) const
	{
		glUniformMatrix4fv(GetUniformLoc(name), len, GL_FALSE, (GLfloat*)data);
	}
}
