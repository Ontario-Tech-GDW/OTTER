/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Shader.h
Utility classes for managing OpenGL shaders and shader programs.
*/

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "glad/glad.h"

namespace nou
{
	//This just helps us to identify the type of information GL spits out in the
	//event of an error compiling a shader or linking a shader program.
	enum class GLInfoLogType
	{
		SHADER,
		PROGRAM
	};

	//This loads in the file specifed as an array of GL characters.
	//This function allocates memory - it is the responsibility of the caller
	//to clean up the data pointer after they have used it!
	bool LoadFileGLChar(const std::string& path, GLchar*& dataOut, GLint& lenOut);

	//This will format and print an info log given by OpenGL in the event of issues
	//compiling shaders and linking shader programs.
	void PrintGLInfoLog(const std::string& preamble, GLInfoLogType logType, GLuint objID, GLint buflen);

	class Shader
	{
		public:

		Shader(const std::string& file, GLenum shaderType);
		~Shader();

		GLuint GetID();

		protected:

		//The OpenGL ID of our shader object.
		GLuint m_id;
	};

	class ShaderProgram
	{
		public:

		ShaderProgram(const std::vector<Shader*>& shaders);
		~ShaderProgram();

		//Calling this will make the object it is called on the
		//current shader program.
		void Bind() const;

		//Fetches the shader program currently in use.
		static const ShaderProgram* Current();

		//Utility functions for managing uniforms - variables
		//we send to the shader that persist until we change them.
		GLint GetUniformLoc(const std::string& name) const;

		template<typename T>
		void SetUniform(const std::string& name, const T& value) const;

		template<typename T>
		void SetUniformArray(const std::string& name, T* data, int len) const;

		protected:

		//The OpenGL ID of our shader program.
		GLuint m_id;

		//The shader program currently in use.
		static const ShaderProgram* m_current;

		void Link();
	};
}