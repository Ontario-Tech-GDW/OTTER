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

		size_t extIndex = filename.find('.');
		
		if (extIndex == std::string::npos || extIndex >= filename.length() - 1)
		{
			err = "Filename specified incorrectly - no extension!";
			return false;
		}

		std::string ext = filename.substr(extIndex + 1, std::string::npos);

		if (ext != "gltf" && ext != "glb")
		{
			err = "Filename specified incorrectly - not a GLTF or GLB!";
			return false;
		}

		bool binary = ext == "glb";

		bool result = (binary) ? 
					  loader->LoadBinaryFromFile(&gltf, &tinygltfErr, &tinygltfWarn, filename.c_str())
					: loader->LoadASCIIFromFile(&gltf, &tinygltfErr, &tinygltfWarn, filename.c_str());

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

		std::vector<glm::vec3> verts;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		bool hasNormals = true, hasUVs = true;

		for (size_t i = 0; i < meshData.primitives.size(); ++i)
		{
			if(!ProcessPrimitive(gltf, i, verts, uvs, normals, 
						         flipUVY, hasNormals, hasUVs, err, warn))
				return false;
		}

		mesh.SetVerts(verts);

		if(hasNormals)
			mesh.SetNormals(normals);

		if(hasUVs)
			mesh.SetUVs(uvs);

		return true;
	}

	bool ProcessPrimitive(const tinygltf::Model& gltf, size_t geomIndex,
		                  std::vector<glm::vec3>& verts, std::vector<glm::vec2>& uvs,
		                  std::vector<glm::vec3>& normals, bool flipUVY,
						  bool& hasNormals, bool& hasUVs,
		                  std::string& err, std::string& warn)
	{
		const tinygltf::Primitive geom = gltf.meshes[0].primitives[geomIndex];

		if (geom.indices == -1)
		{
			err = "File is missing primitive indices. "\
				"Consider changing your GLTF export settings, or else this loader " \
				"must be augmented to support files without indices.";

			return false;
		}

		//glTF stores data per-vertex.
		//This indexer will allow us to access the data that tells
		//us which vertices make up the faces of the object.
		//This allows us to specify our data in the order we need it
		//for OpenGL vertex buffers - in other words, spelling out the vertex
		//data as a set of triangles.
		DataGetter faceIndexer = BuildGetter(gltf, geom.indices);

		if (faceIndexer.elementSize != sizeof(GLshort))
		{
			err = "Primitive indices are in a currently unsupported format. " \
				"Consider changing your GLTF export settings, or else this loader " \
				"must be augmented to support the provided format.";

			return false;
		}

		int vID = FindAccessor(geom, "POSITION");

		if (vID == -1)
		{
			err = "No vertex positions found in mesh primitive " + std::to_string(geomIndex);
			return false;
		}

		int nID = FindAccessor(geom, "NORMAL");
		hasNormals = hasNormals && nID != -1;

		if (!hasNormals)
			warn += "\nNo normals found in mesh primitive " + std::to_string(geomIndex);

		int uvID = FindAccessor(geom, "TEXCOORD_0");
		hasUVs = hasUVs && uvID != -1;

		if (uvID == -1)
			warn += "\nNo UVs found in mesh primitive " + std::to_string(geomIndex);

		DataGetter vGetter, nGetter, uvGetter;

		vGetter = BuildGetter(gltf, vID);

		if (vGetter.elementSize != sizeof(glm::vec3))
		{
			err = "Vertex position data is in a currently unsupported format. " \
				"Consider changing your GLTF export settings, or else this loader " \
				"must be augmented to support the provided format.";

			return false;
		}

		if (hasNormals)
		{
			nGetter = BuildGetter(gltf, nID);

			if (nGetter.elementSize != sizeof(glm::vec3))
			{
				hasNormals = false;
				warn += "\nNormal data is in a currently unsupported format. " \
					"Consider changing your GLTF export settings, or else this loader " \
					"must be augmented to support the provided format.";
			}
		}


		if (hasUVs)
		{
			uvGetter = BuildGetter(gltf, uvID);

			if (uvGetter.elementSize != sizeof(glm::vec2))
			{
				hasUVs = false;
				warn += "\nUV data is in a currently unsupported format. " \
					"Consider changing your GLTF export settings, or else this loader " \
					"must be augmented to support the provided format.";
			}
		}

		size_t startIndex = verts.size();

		verts.resize(verts.size() + faceIndexer.len);

		if (hasNormals)
			normals.resize(normals.size() + faceIndexer.len);

		if (hasUVs)
			uvs.resize(uvs.size() + faceIndexer.len);

		//This is the bit where we actually get to extracting our data.
		for (size_t i = startIndex, f = 0; i < startIndex + faceIndexer.len && f < faceIndexer.len; ++i, ++f)
		{
			//What vertex do we need to look at?
			GLshort vertIndex;
			memcpy(&vertIndex, &faceIndexer.data[f * faceIndexer.stride], sizeof(GLshort));

			size_t vert = vertIndex;

			//Grab our vertex position.
			memcpy(&verts[i], &vGetter.data[vert * vGetter.stride], sizeof(glm::vec3));

			//Grab our vertex normal.
			if (hasNormals)
				memcpy(&normals[i], &nGetter.data[vert * nGetter.stride], sizeof(glm::vec3));

			//Grab our texture coordinates.
			if (hasUVs)
			{
				memcpy(&uvs[i], &uvGetter.data[vert * uvGetter.stride], sizeof(glm::vec2));

				//We may need to flip our vertical UV-coordinate.
				//You will probably need to do this, depending on your export settings/texture.
				if (flipUVY)
					uvs[i].y = 1.0f - uvs[i].y;
			}
		}

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