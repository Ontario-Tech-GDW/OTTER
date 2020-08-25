//////////////////////////////////////////////////////////////////////////
//
// This header is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this header in your GDW games.
//
// This header contains a helper class for drawing the primitive types that
// were originally supported by GLUT. This file was purposefully built to
// be non-optimal
//
// Based off of TTK by Michael Gharbharan 2017
// Shawn Matthews 2019
//
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "TTKContext.h"

namespace TTK {
	namespace Impl {
		class MeshHelper {			
		public:
			~MeshHelper();
			MeshHelper();
			void RenderTeapot(const glm::mat4& transform, const glm::vec4& color) const;
			void RenderSphere(const glm::mat4& transform, const glm::vec4& color) const;
			void RenderCube(const glm::mat4& transform, const glm::vec4& color) const;
			
		private:
			struct mesh {
				GLuint VAO;
				GLuint VBO;
			};
			mesh __MakeMesh(const float* data, size_t size) const;
			
			mesh m_Teapot;
			mesh m_Sphere;
			mesh m_Cube;
			GLuint m_Shader;
		};
	}
}