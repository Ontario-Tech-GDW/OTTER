#include "NotObjLoader.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

<<<<<<< HEAD

=======
>>>>>>> master
// Borrowed from https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
#pragma region String Trimming

// trim from start (in place)
static inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

#pragma endregion 

<<<<<<< HEAD

=======
>>>>>>> master
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
	
	// TODO: Load from file
<<<<<<< HEAD
	while (std::getline(file, line)) {
		trim(line);
		
		if (line.substr(0, 1) == "#") 
		{
			
			// Comment, no-op   
		}    

		
		else if(line.substr(0, 5) == "cube ") // We can do equality check this way since the left side is a string and not a char*  
		{
			std::istringstream ss = std::istringstream(line.substr(5));       
			glm::vec3 pos;
			ss >> pos.x >> pos.y >> pos.z;
			
			glm::vec3 scale;
			ss >> scale.x >> scale.y >> scale.z;
			
			glm::vec3 eulerDeg;
			ss >> eulerDeg.x >> eulerDeg.y >> eulerDeg.z;
			
			glm::vec4 color = glm::vec4(1.0f);
			
			if (ss.rdbuf()->in_avail() > 0) 
			{    
				ss >> color.r >> color.g >> color.b;
			}
			
			if (ss.rdbuf()->in_avail() > 0) 
			{    
				ss >> color.a;
			
			}
			
			MeshFactory::AddCube(mesh, pos, scale, eulerDeg, color);   

		}  

		else if(line.substr(0, 5) == "cube ") // We can do equality check this way since the left side is a string and not a char*    
		{
			std::istringstream ss = std::istringstream(line.substr(5));       
			
			glm::vec3 pos;

			ss >> pos.x >> pos.y >> pos.z;

			glm::vec3 scale;

			ss >> scale.x >> scale.y >> scale.z;

			glm::vec3 eulerDeg;

			ss >> eulerDeg.x >> eulerDeg.y >> eulerDeg.z;

			glm::vec4 color = glm::vec4(1.0f);

			if (ss.rdbuf()->in_avail() > 0) 
			{   
				ss >> color.r >> color.g >> color.b;

			}
			
			if (ss.rdbuf()->in_avail() > 0) 
			{    
				ss >> color.a;
			}
			
			MeshFactory::AddCube(mesh, pos, scale, eulerDeg, color);  
		}    // . . . continued below
			
		else if(line.substr(0, 7) == "sphere ") 
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
			
			if (ss.rdbuf()->in_avail() > 0) 
			{ 
				ss >> color.r >> color.g >> color.b; 
			}
			
			if (ss.rdbuf()->in_avail() > 0) 
			{ 
				ss >> color.a; 
			}
			
			if (mode == "ico") 
			{ 
				MeshFactory::AddIcoSphere(mesh, pos, radii, tesselation, color); 
			} 
			
			else if (mode == "uv") 
			{ 
				MeshFactory::AddUvSphere(mesh, pos, radii, tesselation, color); 
			} 
		}
	}
=======

>>>>>>> master
	// Note: with actual OBJ files you're going to run into the issue where faces are composited of different indices
	// You'll need to keep track of these and create vertex entries for each vertex in the face
	// If you want to get fancy, you can track which vertices you've already added

	return mesh.Bake();
}
