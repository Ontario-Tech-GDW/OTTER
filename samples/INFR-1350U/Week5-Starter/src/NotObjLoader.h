#pragma once

#include "MeshBuilder.h"
#include "MeshFactory.h"

class NotObjLoader
{
public:
	static VertexArrayObject::sptr LoadFromFile(const std::string& filename);

protected:
	NotObjLoader() = default;
	~NotObjLoader() = default;
};