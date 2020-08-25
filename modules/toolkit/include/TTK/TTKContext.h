//////////////////////////////////////////////////////////////////////////
//
// This header is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this header in your GDW games.
//
// This header contains the underlying implementation for batching
// primitive calls, as well as manages the Font Renderer and Mesh Helper
// classes
//
// Based off of TTK by Michael Gharbharan 2017
// Shawn Matthews 2019
//
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <GLM/glm.hpp>
#include "FontRenderer.h"

namespace TTK
{
	namespace Impl {
		class MeshHelper;
	}
	
	class Context {
	public:
		struct SimpleVert {
			glm::vec3 Position;
			glm::vec4 Color;
		};

		struct PointVert
		{
			glm::vec3 Position;
			glm::vec4 Color;
			float     Size;
		};
		
		inline static Context& Instance() {
			if (m_Instance == nullptr)
				m_Instance = new Context();
			return *m_Instance;
		}
		inline static void DestroyContext() {
			delete m_Instance;
			m_Instance = nullptr;
		}
	private:
		static Context* m_Instance;

	public:
		~Context();

		void SetProjection(const glm::mat4& value) { m_Projection = value; m_ViewProjection = m_Projection * m_ViewMatrix; }
		const glm::mat4& GetProjection() const { return m_Projection; }

		void SetView(const glm::mat4& value) { m_ViewMatrix = value; m_ViewProjection = m_Projection * m_ViewMatrix; }
		const glm::mat4& GetView() const { return m_ViewMatrix; }

		const glm::mat4& GetViewProjection() const { return m_ViewProjection; }
		
		glm::mat4 GetOrthoProjection() const;
		
		void SetWindowSize(int windowWidth, int windowHeight);

		void RenderText(const char* text, const glm::vec2& position, const glm::vec4& color, float scale = 1.0f);
		
		void DrawTeapot(const glm::mat4& mat, const glm::vec4& color = glm::vec4(1.0f)) const;
		void DrawSphere(const glm::mat4& mat, const glm::vec4& color = glm::vec4(1.0f)) const;
		void DrawCube(const glm::mat4& mat, const glm::vec4& color = glm::vec4(1.0f)) const;

		void AddLine(const glm::vec3& a, const glm::vec3& b, const glm::vec4& color = {0, 0, 0, 1});
		void AddTri(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec4& color = { 0, 0, 0, 1 });
		void AddQuad(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color = { 0, 0, 0, 1 });
		void AddPoint(const glm::vec3& pos, float size, const glm::vec4& color = { 0, 0, 0, 1 });
		
		void Flush();

	private:
		Context();
		glm::mat4				  m_Projection;
		glm::mat4                 m_ViewMatrix;
		glm::mat4                 m_ViewProjection;
		TTK::TrueTypeTextureFont* m_DefaultFont;
		Impl::MeshHelper*         m_MeshHelper;

		GLuint m_ShaderHandle;
		GLuint m_PointShaderHandle;
		struct GLBuff {
			GLuint VBO, VAO;
			size_t Count;
			size_t ElemSize;
			GLenum Mode;
			void*  Data;
			GLuint Shader;
		};
		GLBuff m_Tris, m_Lines, m_Points;

		int m_WindowWidth, m_WindowHeight;

		GLBuff __InitBuff(GLenum mode, GLuint shader, void* dataSource, size_t elemSize, size_t maxElems);
		void __Flush(GLBuff& buff);
		GLuint __CompileShader(const char* vsSource, const char* fsSource);

		static const size_t MaxPointVerts = 512;
		static const size_t MaxLineVerts = 512 * 2;
		static const size_t MaxTriVerts = 512 * 3;

		PointVert  m_PointVerts[MaxPointVerts];
		SimpleVert m_LineVerts[MaxLineVerts];
		SimpleVert m_TriVerts[MaxTriVerts];
	};
}
