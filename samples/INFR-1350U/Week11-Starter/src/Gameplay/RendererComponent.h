#pragma once
#include "Graphics/VertexArrayObject.h"
#include "Gameplay/ShaderMaterial.h"

class RendererComponent {
public:
	VertexArrayObject::sptr Mesh;
	ShaderMaterial::sptr    Material;

	RendererComponent& SetMesh(const VertexArrayObject::sptr& mesh) { Mesh = mesh; return *this; }
	RendererComponent& SetMaterial(const ShaderMaterial::sptr& material) { Material = material; return *this; }
};