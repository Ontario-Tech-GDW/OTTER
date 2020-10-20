#include "ObjLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

VertexArrayObject::sptr ObjLoader::LoadFromFile(const std::string& filename)
{
	//TODO: Worry about checking for more spaces later

	//struct 'Face' is created to store the different value types between vertices, textures, and normals
	struct Face {

		//struct member variables 
		glm::vec3 vertices;
		glm::vec2 textures;
		glm::vec3 normals;
	};

	//ifstream variable
	std::ifstream file;
	
	//String variable to 
	std::string line;

	//Vector vraiables to store the Vertices, Normals and Textures
	std::vector<glm::vec3> objVertices;
	std::vector < glm::vec2> objTextures;
	std::vector < glm::vec3> objNormals;

	//Vector of the struct that stores the values for each face in a list
	std::vector<Face> objFaces;

	//Open a file
	file.open(filename);

	//Error check to see if the file fails to open
	if (!file) {
		std::cout << "File could not load";
	}


	//While loop that iterates through to the end of the file to determine the obj's vertices, textures and normals
	while (std::getline(file, line))
	{

		if (line.substr(0, 1) == "#")
		{
			//Do nothing. The pound symbol represents comments
		}

		//If the text file begins with v we stringstream the text and pushback the values of the vertices in a vec3
		else if (line.substr(0, 2) == "v ")
		{

			std::istringstream ss = std::istringstream(line.substr(2));
			glm::vec3 v;


			ss >> v.x >> v.y >> v.z;
			
			//vertices stored in vec3 'v' are added to the list of textures in the vector objTextures
			objVertices.push_back(v);

		}

		//If the text file begins with v we stringstream the text and pushback the values of the vertices in a vec2
		//Textures are stored in vec2's as they only have x and y values
		else if (line.substr(0, 3) == "vt ")
		{
			glm::vec2 vt;

			std::istringstream ss = std::istringstream(line.substr(3));
			ss >> vt.x >> vt.y;

			//textures stored in vec2 'vt' are added to the list of textures in the vector objTextures
			objTextures.push_back(vt);

		}

		//If the text file begins with v we stringstream the text and pushback the values of the vertices in a vec3
		else if (line.substr(0, 3) == "vn ")
		{
			glm::vec3 vn;

			std::istringstream ss = std::istringstream(line.substr(3));
			

			ss >> vn.x >> vn.y >> vn.z;

			//normals stored in vec3 'vn' are added to the list of normals in the vector objNormals
			objNormals.push_back(vn);

		}

		else if (line.substr(0,2) == "f ")
		{

			//Replace function to get rid of all back slashes and change them with a space
			//This allows for the program to read the index's for the face value's easily
			std::replace(line.begin(), line.end(), '/', ' ');

			std::istringstream ss = std::istringstream(line.substr(2));

			//for loop that iterates through each set of vertices, textures and normals within a face
			for (int i = 0; i < 3; i++)
			{


				//size_t variables that represent each index for a face within the obj file
				size_t index1, index2, index3;

				ss >> index1 >> index2 >> index3;

				//Create a new struct variable
				Face updatedFaces;

				//struct variable updatedFaces gets access to the values stored in each vector
				//Subtract the index by 1 so there is no out of bounds error
				updatedFaces.vertices = objVertices[index1 - 1];
				updatedFaces.textures = objTextures[index2 - 1];
				updatedFaces.normals = objNormals[index3 - 1];

				//vector of struct Faces push's back the values recieved from 'updatedFaces'
				objFaces.push_back(updatedFaces);

			}



		}
		

	}

	//Float vector to store values of each face that will be passed to OpenGL
	std::vector<float> valToPass;

	//for loop that iterates through the values for each face within the object
	for ( int i = 0; i < objFaces.size(); i++)
	{
		//Stores vertices in the vector
		valToPass.push_back(objFaces[i].vertices.x);
		valToPass.push_back(objFaces[i].vertices.y);
		valToPass.push_back(objFaces[i].vertices.z);

		//Stores textures in the vector
		valToPass.push_back(objFaces[i].textures.x);
		valToPass.push_back(objFaces[i].textures.y);

		//Stores normals in the vector
		valToPass.push_back(objFaces[i].normals.x);
		valToPass.push_back(objFaces[i].normals.y);
		valToPass.push_back(objFaces[i].normals.z);

	}

	//Create vertex buffer
	VertexBuffer::sptr interleaved_vbo = VertexBuffer::Create();

	//Data function points to first index in the valToPass array
	interleaved_vbo->LoadData(valToPass.data(), valToPass.size() );

	//References the VertexPosNormtex in VertexTypes.h
	size_t stride = sizeof(VertexPosNormTex);
	VertexArrayObject::sptr vao = VertexArrayObject::Create();

	//references the VertxPosNormFunction::V_DECL in VertexTypes.cpp
	vao->AddVertexBuffer(interleaved_vbo, VertexPosNormTex::V_DECL);
	
	


	return vao;
}
