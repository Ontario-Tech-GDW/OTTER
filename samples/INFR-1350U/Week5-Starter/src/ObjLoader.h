#pragma once
#pragma once

#include "MeshBuilder.h"
#include "MeshFactory.h"

class ObjLoader
{
public:
	//LoadFromFile function is created as a shared point of a VAO. This function gets access to the file
	static VertexArrayObject::sptr LoadFromFile(const std::string& filename);

protected:
	ObjLoader() = default;
	~ObjLoader() = default;
};
