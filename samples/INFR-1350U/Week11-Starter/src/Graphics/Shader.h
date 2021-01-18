#pragma once
#include <glad/glad.h>
#include <memory>

#include <string>               // for std::string
#include <unordered_map>        // for std::unordered_map
#include <GLM/glm.hpp>          // for our GLM types
#include <GLM/gtc/type_ptr.hpp> // for glm::value_ptr
#include "Logging.h"            // for the logging functions

/// <summary>
/// This class will wrap around an OpenGL shader program
/// </summary>
class Shader final
{
public:
	typedef std::shared_ptr<Shader> sptr;
	static inline sptr Create() {
		return std::make_shared<Shader>(); 
	}
	
public:
	// We'll disallow moving and copying, since we want to manually control when the destructor is called
	// We'll use these classes via pointers
	Shader(const Shader& other) = delete;
	Shader(Shader&& other) = delete;
	Shader& operator=(const Shader& other) = delete;
	Shader& operator=(Shader&& other) = delete;

public:
	/// <summary>
	/// Creates a new empty shader object
	/// </summary>
	Shader();

	// Note, we don't need to make this virtual since this class is marked final (basically it can't be used as a base class)
	~Shader();

	/// <summary>
	/// Loads a single shader stage into this shader object (ex: Vertex Shader or Fragment Shader)
	/// </summary>
	/// <param name="source">The source code of the shader to load</param>
	/// <param name="type">The stage to load (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)</param>
	/// <returns>True if the shader is loaded, false if there was an issue</returns>
	bool LoadShaderPart(const char* source, GLenum type);
	/// <summary>
	/// Loads a single shader stage into this shader object (ex: Vertex Shader or Fragment Shader) from an external file (in res)
	/// </summary>
	/// <param name="path">The relative path to the file containing the source</param>
	/// <param name="type">The stage to load (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)</param>
	/// <returns>True if the shader is loaded, false if there was an issue</returns>
	bool LoadShaderPartFromFile(const char* path, GLenum type);

	/// <summary>
	/// Links the vertex and fragment shader, and allows this shader program to be used
	/// </summary>
	/// <returns>True if the linking was sucessful, false if otherwise</returns>
	bool Link();

	/// <summary>
	/// Binds this shader for use
	/// </summary>
	void Bind();
	/// <summary>
	/// Unbinds all shader programs
	/// </summary>
	static void UnBind();

	/// <summary>
	/// Gets the underlying OpenGL handle that this class is wrapping
	/// </summary>
	GLuint GetHandle() const { return _handle; }
	
public:
	int GetUniformLocation(const std::string& name);
	
	template <typename T>
	void SetUniform(const std::string& name, const T& value) {
		int location = GetUniformLocation(name);
		if (location != -1) {
			SetUniform(location, &value, 1);
		}
	}
	template <typename T>
	void SetUniformMatrix(const std::string& name, const T& value, bool transposed = false) {
		int location = GetUniformLocation(name);
		if (location != -1) {
			SetUniformMatrix(location, &value, 1, transposed);
		}
	}
	template <typename T>
	void SetUniform(int location, const T& value) {
		if (location != -1) {
			SetUniform(location, &value, 1);
		}
	}
	template <typename T>
	void SetUniformMatrix(int location, const T& value, bool transposed = false) {
		if (location != -1) {
			SetUniformMatrix(location, &value, 1, transposed);
		}
	}
	
	void SetUniformMatrix(int location, const glm::mat3* value, int count = 1, bool transposed = false);
	void SetUniformMatrix(int location, const glm::mat4* value, int count = 1, bool transposed = false);
	void SetUniform(int location, const float* value, int count = 1);
	void SetUniform(int location, const glm::vec2* value, int count = 1);
	void SetUniform(int location, const glm::vec3* value, int count = 1);
	void SetUniform(int location, const glm::vec4* value, int count = 1);
	void SetUniform(int location, const int* value, int count = 1);
	void SetUniform(int location, const glm::ivec2* value, int count = 1);
	void SetUniform(int location, const glm::ivec3* value, int count = 1);
	void SetUniform(int location, const glm::ivec4* value, int count = 1);
	void SetUniform(int location, const bool* value, int count = 1);
	void SetUniform(int location, const glm::bvec2* value, int count = 1);
	void SetUniform(int location, const glm::bvec3* value, int count = 1);
	void SetUniform(int location, const glm::bvec4* value, int count = 1);
	
protected:
	GLuint _vs;
	GLuint _fs;
	
	GLuint _handle;

	std::unordered_map<std::string, int> _uniformLocs;
	
};