/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

GLObjects.h
Classes for managing OpenGL vertex buffers and vertex array objects.
You'll be learning a LOT more about this in your graphics class.
*/

#pragma once

#include <memory>
#include <vector>
#include <map>
#include <string>

#include "glad/glad.h"

namespace nou
{
	//Class for managing OpenGL Vertex Buffer Objects (VBOs).
	//A vertex buffer stores a hunk of data for OpenGL on the GPU.
	//This might be a list of vertex positions, texture coordinates, etc.
	//As implemented, if you want to use these in a container, you MUST
	//use a pointer (e.g., std::vector<VertexBuffer> is not okay,
	//but std::vector<std::unique_ptr<VertexBuffer>> is good).
	class VertexBuffer
	{
		public:

		template<typename T>
		VertexBuffer(GLint elementLen, const std::vector<T>& data, bool dynamic = false)
		{
			m_elementLen = elementLen;
			m_startIndex = 0;
			m_len = 0;
			m_dynamic = dynamic;

			glGenBuffers(1, &m_id);
			UpdateData(data);
		}

		~VertexBuffer()
		{
			glDeleteBuffers(1, &m_id);
		}

		//This is called a copy constructor.
		//The delete keyword tells the compiler we don't want to allow this object
		//to be copied.
		//This is because it doesn't make sense for us to copy a vertex buffer,
		//since we are not storing a copy of the data on the CPU.
		//Depending on your implementation of vertex buffers, it might make sense
		//for you, though!
		VertexBuffer(const VertexBuffer&) = delete;

		/*This is called a move constructor. This would be necessary if you wanted
		to put VertexBuffers into a container without using pointers.

		VertexBuffer(VertexBuffer&& other) noexcept
		{
			m_id = other.m_id;
			m_elementLen = other.m_elementLen;
			m_elementSize = other.m_elementSize;
			m_startIndex = other.m_startIndex;
			m_len = other.m_len;

			other.m_id = 0;
		}
		*/

		GLsizei Length() const { return m_len; }

		GLsizei ElementSize() const { return m_elementSize; }

		GLsizei ElementLength() const { return m_elementLen; }

		GLsizei StartIndex() const { return m_startIndex; }

		GLuint GetID() const { return m_id; }

		//This uploads the data specified into our OpenGL buffer on the GPU.
		template<typename T>
		void UpdateData(const std::vector<T>& data)
		{
			m_len = (GLsizei)data.size();
			m_elementSize = sizeof(T);

			GLenum usage = (m_dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

			glBindBuffer(GL_ARRAY_BUFFER, m_id);
			glBufferData(GL_ARRAY_BUFFER, m_len * m_elementSize, &(data[0]), usage);
		}

		protected:

		//The OpenGL ID of our VBO.
		GLuint m_id;

		//The number of components in a single data point (e.g., Vector3 = 3 components).
		GLint m_elementLen;
		//The size of a single data point in bytes.
		GLsizei m_elementSize;

		//The number of data points in our buffer.
		GLsizei m_len;

		//Any offset we should take to get to the "first" element in our buffer.
		//(Usually this will be 0 unless you are doing something Fancy(TM).)
		GLsizei m_startIndex;

		//Whether we expect to update this data frequently.
		bool m_dynamic;
	};

	//Class for managing OpenGL Vertex Array Objects (VAOs).
	//Just as with VertexBuffer, as written, this class is intended to be used via pointers.
	class VertexArray
	{
		public:

		enum class DrawMode
		{
			POINTS = GL_POINTS,
			LINES = GL_LINES,
			LINE_STRIP = GL_LINE_STRIP,
			LINE_LOOP = GL_LINE_LOOP,
			TRIANGLES = GL_TRIANGLES,
			TRIANGLE_STRIP = GL_TRIANGLE_STRIP
		};

		DrawMode m_drawMode;

		VertexArray()
		{
			m_drawMode = DrawMode::TRIANGLES;
			glGenVertexArrays(1, &m_id);
			m_len = 0;
		}

		~VertexArray()
		{
			glDeleteVertexArrays(1, &m_id);
		}

		/*The functions commented out here would be necessary if you wanted
		to use this class without going through pointers.
		
		This is called a move constructor.
		VertexArray(VertexArray&& other) noexcept
		{
			m_id = other.m_id;
			m_len = other.m_len;
			m_drawMode = other.m_drawMode;

			for (auto& [attribLoc, buf] : other.m_vbos)
			{
				BindAttrib(*buf, attribLoc);
			}

			other.m_id = 0;
		}

		This is called a copy constructor.
		Unlike with VertexBuffers, it might make sense for us to make
		a copy of a VertexArray (e.g., when rendering multiple animated
		meshes with the same base data). 
		VertexArray(const VertexArray& other)
		{
			glGenVertexArrays(1, &m_id);
			m_len = other.m_len;
			m_drawMode = other.m_drawMode;

			for (auto& [attribLoc, buf] : other.m_vbos)
			{
				BindAttrib(*buf, attribLoc);
			}
		}

		This is called a move assignment operator.
		VertexArray& operator=(VertexArray&& other)
		{
			if (this != &other)
			{
				glDeleteVertexArrays(1, &m_id);
				m_vbos.clear();

				m_id = other.m_id;
				m_len = other.m_len;
				m_drawMode = other.m_drawMode;

				for (auto& [attribLoc, buf] : other.m_vbos)
				{
					BindAttrib(*buf, attribLoc);
				}

				other.m_id = 0;
			}

			return *this;
		}

		This is called a copy assignment operator.
		VertexArray& operator=(const VertexArray& other)
		{
			if (this != &other)
			{
				glDeleteVertexArrays(1, &m_id);
				m_vbos.clear();

				glGenVertexArrays(1, &m_id);
				m_len = other.m_len;
				m_drawMode = other.m_drawMode;

				for (auto& [attribLoc, buf] : other.m_vbos)
				{
					BindAttrib(*buf, attribLoc);
				}
			}

			return *this;
		}*/

		//This associates a VertexBuffer with our vertex array object.
		//In other words, it tells OpenGL that whenever we draw our "thing"
		//that this VAO represents, we need the data associated with the
		//buffer specified to be found in the location specified.
		void BindAttrib(const VertexBuffer& buf, GLuint attribLoc)
		{
			m_vbos[attribLoc] = &buf;

			m_len = buf.Length();

			glBindVertexArray(m_id);
			glEnableVertexAttribArray(attribLoc);
			glBindBuffer(GL_ARRAY_BUFFER, buf.GetID());
			glVertexAttribPointer(attribLoc, buf.ElementLength(), 
								  GL_FLOAT, GL_FALSE, 0,
								 reinterpret_cast<void*>((long long)buf.StartIndex() *
														 (long long)buf.ElementSize()));
		}

		void SetDrawMode(DrawMode drawMode)
		{
			m_drawMode = drawMode;
		}

		void Draw()
		{
			m_len = m_vbos.begin()->second->Length();

			glBindVertexArray(m_id);
			glDrawArrays((int)m_drawMode, 0, m_len);
		}

		void DrawElements(const std::vector<GLuint>& indices, size_t count)
		{
			if (count == 0)
				return;

			glBindVertexArray(m_id);
			glDrawElements((int)m_drawMode,
						   static_cast<GLsizei>(count),
						   GL_UNSIGNED_INT,
						   &(indices[0]));
		}

		protected:

		//The OpenGL ID of our VAO.
		GLuint m_id;

		//The number of elements in our VAO (typically equals the number of vertices in a 3D model).
		GLsizei m_len;

		//A record of the VBOs associated with this VAO.
		std::map<GLint, const VertexBuffer*> m_vbos;
	};
}

