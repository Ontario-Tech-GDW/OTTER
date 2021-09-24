#pragma once
#include <glad/glad.h>

// We can use an enum to make our code more readable and restrict
// values to only ones we want to accept
enum class ShaderPartType {
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	Unknown = GL_NONE // Usually good practice to have an "unknown" or "none" state for enums
};

/// <summary>
/// This class will wrap around an OpenGL shader program
/// </summary>
class Shader final
{
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
	bool LoadShaderPart(const char* source, ShaderPartType type);
	/// <summary>
	/// Loads a single shader stage into this shader object (ex: Vertex Shader or Fragment Shader) from an external file (in res)
	/// </summary>
	/// <param name="path">The relative path to the file containing the source</param>
	/// <param name="type">The stage to load (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)</param>
	/// <returns>True if the shader is loaded, false if there was an issue</returns>
	bool LoadShaderPartFromFile(const char* path, ShaderPartType type);

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
	static void Unbind();

	/// <summary>
	/// Gets the underlying OpenGL handle that this class is wrapping
	/// </summary>
	GLuint GetHandle() const { return _handle; }
	
protected:
	// Stores the vertex and fragment shader handles
	GLuint _vs;
	GLuint _fs;
	
	// Stores the shader program handle
	GLuint _handle;
};