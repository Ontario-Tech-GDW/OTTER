/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Mesh.h
Very basic class for managing the data associated with a 3D model.
*/

#pragma once

#include "GLObjects.h"

#include "GLM/glm.hpp"

#include <vector>
#include <string>
#include <map>
#include <memory>

namespace nou
{
	class Mesh
	{
		public:

		//This enumerator corresponds to the layout locations we use as a 
		//convention for passing attributes to our vertex shaders.
		enum class Attrib
		{
			POSITION = 0,
			NORMAL = 1,
			UV = 2,
			JOINT_INFLUENCE = 3,
			SKIN_WEIGHT = 4
		};

		Mesh() = default;
		virtual ~Mesh() = default;

		void SetVerts(const std::vector<glm::vec3>& verts);
		void SetNormals(const std::vector<glm::vec3>& normals);
		void SetUVs(const std::vector<glm::vec2>& uvs);

		//Fetches a vertex buffer associated with the desired attribute.
		//Used by mesh rendering components to grab the requisite data
		//associated with this model in OpenGL.
		const VertexBuffer* GetVBO(Attrib attrib) const;

		protected:

		std::vector<glm::vec3> m_verts;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec2> m_uvs;

		std::map<Attrib, std::unique_ptr<VertexBuffer>> m_vbo;

		//Sets up a VertexBuffer for the desired attribute.
		template<typename T>
		void SetVBO(Attrib attrib, GLint elementLen, const std::vector<T>& data)
		{
			//We shouldn't be trying to send an empty array!
			//A VBO with no data would just lead to memory access errors.
			if (data.size() == 0)
			{
				m_vbo.erase(attrib);
				return;
			}

			auto it = m_vbo.find(attrib);

			//If our VBO does not already exist, make a new one.
			if (it == m_vbo.end())
				m_vbo.insert({attrib,
					std::make_unique<VertexBuffer>(elementLen, data)});
			//If our VBO does exist, update it with the new data specified.
			else
				it->second->UpdateData(data);
		}
	};
}