#include "NotObjLoader.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "StringUtils.h"

VertexArrayObject::sptr NotObjLoader::LoadFromFile(const std::string& filename)
{
	// Open our file in binary mode
	std::ifstream file;
	file.open(filename, std::ios::binary);

	// If our file fails to open, we will throw an error
	if (!file) {
		throw std::runtime_error("Failed to open file");
	}

	MeshBuilder<VertexPosNormTexCol> mesh;
	std::string line;
	
	// Iterate as long as there is content to read
	while (std::getline(file, line)) {
		trim(line);
		if (line.substr(0, 1) == "#")
		{
			// Comment, no-op
		}
		else if (line.substr(0, 5) == "cube ") // We can do equality check this way since the left side is a string and not a char*
		{
			std::istringstream ss = std::istringstream(line.substr(5));
			glm::vec3 pos;
			ss >> pos.x >> pos.y >> pos.z;
			
			glm::vec3 scale;
			ss >> scale.x >> scale.y >> scale.z;
			
			glm::vec3 eulerDeg;
			ss >> eulerDeg.x >> eulerDeg.y >> eulerDeg.z;
			
			glm::vec4 color = glm::vec4(1.0f);
			if (ss.rdbuf()->in_avail() > 0) {
				ss >> color.r >> color.g >> color.b;
			}
			if (ss.rdbuf()->in_avail() > 0) {
				ss >> color.a;
			}

			MeshFactory::AddCube(mesh, pos, scale, eulerDeg, color);
			
		}
		else if (line.substr(0, 6) == "plane ")
		{
			std::istringstream ss = std::istringstream(line.substr(6));
			glm::vec3 pos;
			ss >> pos.x >> pos.y >> pos.z;
			glm::vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			glm::vec3 tangent;
			ss >> tangent.x >> tangent.y >> tangent.z;
			glm::vec2 size;
			ss >> size.x >> size.y;
			glm::vec4 color = glm::vec4(1.0f);
			if (ss.rdbuf()->in_avail() > 0) {
				ss >> color.r >> color.g >> color.b;
			}
			if (ss.rdbuf()->in_avail() > 0) {
				ss >> color.a;
			}
			
			MeshFactory::AddPlane(mesh, pos, normal, tangent, size, color);
		}
		else if (line.substr(0, 7) == "sphere ")
		{
			std::istringstream ss = std::istringstream(line.substr(7));

			std::string mode;
			ss >> mode;

			int tesselation = 0;
			ss >> tesselation;

			glm::vec3 pos;
			ss >> pos.x >> pos.y >> pos.z;

			glm::vec3 radii;
			ss >> radii.x >> radii.y >> radii.z;
			
			glm::vec4 color = glm::vec4(1.0f);
			if (ss.rdbuf()->in_avail() > 0) {
				ss >> color.r >> color.g >> color.b;
			}
			if (ss.rdbuf()->in_avail() > 0) {
				ss >> color.a;
			}
						
			if (mode == "ico") {
				MeshFactory::AddIcoSphere(mesh, pos, radii, tesselation, color);
			} else if (mode == "uv") {
				MeshFactory::AddUvSphere(mesh, pos, radii, tesselation, color);
			}
		}
	}

	// Note: with actual OBJ files you're going to run into the issue where faces are composited of different indices
	// You'll need to keep track of these and create vertex entries for each vertex in the face
	// If you want to get fancy, you can track which vertices you've already added

	return mesh.Bake();
}
