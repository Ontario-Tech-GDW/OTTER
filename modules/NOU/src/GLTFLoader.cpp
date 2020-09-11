/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

GLTFLoader.cpp
Utility functions for loading models in the glTF format.
Helpful reference: https://www.khronos.org/files/gltf20-reference-guide.pdf
*/

#include "NOU/GLTFLoader.h"

#include <sstream>

#include "tiny_gltf.h"

namespace nou::GLTF
{
	void LoadMesh(const std::string& filename, Mesh& mesh, bool flipUVY)
	{
		auto gltf = std::make_unique<tinygltf::Model>();

		std::string err, warn;

		bool result = ParseGLTF(filename, *gltf, err, warn);

		if (!result)
		{
			DumpErrorsAndWarnings(filename, err, warn);
			return;
		}

		result = ExtractGeometry(*gltf, mesh, flipUVY, err, warn);

		if (!result)
		{
			DumpErrorsAndWarnings(filename, err, warn);
			return;
		}

		DumpErrorsAndWarnings(filename, err, warn);
		printf("Loaded mesh from %s.\n", filename.c_str());
	}

	void DumpErrorsAndWarnings(const std::string& filename,
							   const std::string& err,
							   const std::string& warn)
	{
		if (!err.empty())
			printf("Error extracting mesh from %s: %s\n",
				filename.c_str(), err.c_str());
		if (!warn.empty())
			printf("Warning(s) extracting mesh from %s: %s\n",
				filename.c_str(), warn.c_str());
	}

	bool ParseGLTF(const std::string& filename, tinygltf::Model& gltf, 
				   std::string& err, std::string& warn)
	{
		auto loader = std::make_unique<tinygltf::TinyGLTF>();

		std::string tinygltfErr, tinygltfWarn;
		bool result = loader->LoadASCIIFromFile(&gltf, &tinygltfErr, &tinygltfWarn, filename.c_str());

		if (!tinygltfErr.empty())
		{
			std::ostringstream errStream;
			errStream << tinygltfErr << "\n";
			err = errStream.str();
		}

		if (!tinygltfWarn.empty())
		{
			std::ostringstream warnStream;
			warnStream << tinygltfWarn << "\n";
			warn = warnStream.str();
		}

		if (!result)
			printf("Failed to load .gltf: %s\n", filename.c_str());

		return result;
	}

	bool ExtractGeometry(const tinygltf::Model& gltf, Mesh& mesh, bool flipUVY,
						 std::string& err, std::string& warn)
	{
		if (gltf.meshes.size() == 0)
		{
			err = "No meshes in file.";
			return false;
		}
			
		const tinygltf::Mesh& meshData = gltf.meshes[0];

		if (meshData.primitives.size() == 0)
		{
			err = "No geometry data associated with mesh.";
			return false;
		}

		const tinygltf::Primitive& geom = meshData.primitives[0];

		//glTF stores data per-vertex.
		//This indexer will allow us to access the data that tells
		//us which vertices make up the faces of the object.
		//This allows us to specify our data in the order we need it
		//for OpenGL vertex buffers - in other words, spelling out the vertex
		//data as a set of triangles.
		DataGetter faceIndexer = BuildGetter(gltf, geom.indices);

		if (faceIndexer.elementSize != sizeof(GLshort))
		{
			err = "Primitive indices are in a currently unsupported format." \
				"Consider changing your GLTF export settings, or else check for " \
				"and support this format in your GLTF loader implementation.";

			return false;
		}

		int vID = FindAccessor(geom, "POSITION");

		if (vID == -1)
		{
			err = "No vertex positions found in mesh.";
			return false;
		}

		int nID = FindAccessor(geom, "NORMAL");
		bool hasNormals = nID != -1;

		if (!hasNormals)
			warn += "\nNo normals found in mesh.";

		int uvID = FindAccessor(geom, "TEXCOORD_0");
		bool hasUVs = uvID != -1;

		if (uvID == -1)
			warn += "\nNo UVs found in mesh.";

		DataGetter vGetter, nGetter, uvGetter;

		vGetter = BuildGetter(gltf, vID);

		if (vGetter.elementSize != sizeof(glm::vec3))
		{
			err = "Vertex position data is in a currently unsupported format." \
				  "Consider changing your GLTF export settings, or else check for " \
				  "and support this format in your GLTF loader implementation.";

			return false;
		}

		if (hasNormals)
		{
			nGetter = BuildGetter(gltf, nID);

			if (nGetter.elementSize != sizeof(glm::vec3))
			{
				hasNormals = false;
				warn += "\nNormal data is in a currently unsupported format." \
				   "Consider changing your GLTF export settings, or else check for " \
				   "and support this format in your GLTF loader implementation.";
			}
		}
			

		if(hasUVs)
		{
			uvGetter = BuildGetter(gltf, uvID);

			if (uvGetter.elementSize != sizeof(glm::vec2))
			{
				hasUVs = false;
				warn += "\nUV data is in a currently unsupported format." \
					"Consider changing your GLTF export settings, or else check for " \
					"and support this format in your GLTF loader implementation.";
			}
		}

		std::vector<glm::vec3> verts;
		verts.resize(faceIndexer.len);
		
		std::vector<glm::vec3> normals;

		if(hasNormals)
			normals.resize(faceIndexer.len);

		std::vector<glm::vec2> uvs;

		if(hasUVs)
			uvs.resize(faceIndexer.len);

		//This is the bit where we actually get to extracting our data.
		for (size_t i = 0; i < faceIndexer.len; ++i)
		{
			//What vertex do we need to look at?
			GLshort vertIndex;
			memcpy(&vertIndex, &faceIndexer.data[i * faceIndexer.stride], sizeof(GLshort));

			size_t vert = vertIndex;
			
			//Grab our vertex position.
			memcpy(&verts[i], &vGetter.data[vert * vGetter.stride], sizeof(glm::vec3));
			
			//Grab our vertex normal.
			if(hasNormals)
				memcpy(&normals[i], &nGetter.data[vert * nGetter.stride], sizeof(glm::vec3));

			//Grab our texture coordinates.
			if (hasUVs)
			{
				memcpy(&uvs[i], &uvGetter.data[vert * uvGetter.stride], sizeof(glm::vec2));
				
				//We may need to flip our vertical UV-coordinate.
				//You will probably need to do this, depending on your export settings/texture.
				if(flipUVY)
					uvs[i].y = 1.0f - uvs[i].y;
			}							
		}

		mesh.SetVerts(verts);

		if(hasNormals)
			mesh.SetNormals(normals);

		if(hasUVs)
			mesh.SetUVs(uvs);

		return true;
	}

	int FindAccessor(const tinygltf::Primitive& geom, const std::string& name)
	{
		auto it = geom.attributes.find(name);

		if(it == geom.attributes.end())
			return -1;

		return it->second;
	}

	DataGetter BuildGetter(const tinygltf::Model& gltf, int accIndex)
	{
		const tinygltf::Accessor& acc = gltf.accessors[accIndex];
		const tinygltf::BufferView& bv = gltf.bufferViews[acc.bufferView];
		const tinygltf::Buffer& buf = gltf.buffers[bv.buffer];

		const unsigned char* data = &(buf.data[bv.byteOffset + acc.byteOffset]);
		size_t len = acc.count;
		int stride = acc.ByteStride(bv);
		int size = tinygltf::GetComponentSizeInBytes(acc.componentType) *
				   tinygltf::GetNumComponentsInType(acc.type);

		return { data, len, stride, size };
	}
}