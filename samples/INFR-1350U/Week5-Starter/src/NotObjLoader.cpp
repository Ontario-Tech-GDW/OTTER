#include "NotObjLoader.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

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

	// Note: with actual OBJ files you're going to run into the issue where faces are composited of different indices
	// You'll need to keep track of these and create vertex entries for each vertex in the face
	// If you want to get fancy, you can track which vertices you've already added

	return mesh.Bake();
}
