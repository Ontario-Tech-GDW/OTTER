/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

GLTFLoader.h
Utility functions for loading models in the glTF format.
Helpful reference: https://www.khronos.org/files/gltf20-reference-guide.pdf
*/

#pragma once

#include "Mesh.h"

#include <string>

//Forward declaration of objects defined by the tinyGLTF library.
namespace tinygltf
{
	class Model;
	struct Primitive;
}

namespace nou::GLTF
{
	struct DataGetter
	{
		const unsigned char* data;
		size_t len;
		int stride;
		int elementSize;
	};

	//Loads a 3D model into the mesh object given.
	void LoadMesh(const std::string& filename, Mesh& mesh, bool flipUVY = true);
	
	void DumpErrorsAndWarnings(const std::string& filename,
							   const std::string& err,
							   const std::string& warn);

	//Parses the file with tinyGLTF.
	bool ParseGLTF(const std::string& filename, tinygltf::Model& gltf,
				   std::string& err, std::string& warn);

	//Takes a glTF model and extracts vertex positions, normals, and texture coordinates.
	bool ExtractGeometry(const tinygltf::Model& gltf, Mesh& mesh, bool flipUVY,
					     std::string& err, std::string& warn);

	bool ProcessPrimitive(const tinygltf::Model& gltf, size_t geomIndex, 
					      std::vector<glm::vec3>& verts, std::vector<glm::vec2>& uvs,
						  std::vector<glm::vec3>& normals, bool flipUVY,
						  bool& hasNormals, bool& hasUVs,
						  std::string& err, std::string& warn);

	//Utility functions for more easily accessing data stored in glTF buffers.
	int FindAccessor(const tinygltf::Primitive& geom, const std::string& name);
	DataGetter BuildGetter(const tinygltf::Model& gltf, int accIndex);
}