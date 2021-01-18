#pragma once
#include <string>
#include "Graphics/Shader.h"
#include "Graphics/ITexture.h"
#include "Utilities/Macros.h"
#include <EnumToString.h>

struct ShaderParamName {
	std::string Name;
	int         Location;

	ShaderParamName(const std::string& name) :
		Name(name), Location(-1) {}

	bool operator ==(const ShaderParamName& r) const {
		return Name == r.Name;
	}
	bool operator !=(const ShaderParamName& r) const {
		return Name != r.Name;
	}
};

namespace  std
{
	template<>
	struct hash<ShaderParamName> {
		std::size_t operator()(const ShaderParamName& p) const noexcept {
			return std::hash<std::string>()(p.Name);
		}
	};
}

class ShaderMaterial {
	SMART_MEMORY_MANAGED(ShaderMaterial)
public:
	
	ShaderMaterial();
	virtual ~ShaderMaterial();

	Shader::sptr Shader;
	std::unordered_map<ShaderParamName, ITexture::sptr> Textures;
	std::unordered_map<ShaderParamName, float> FloatParams;
	std::unordered_map<ShaderParamName, glm::vec2> Vec2Params;
	std::unordered_map<ShaderParamName, glm::vec3> Vec3Params;
	std::unordered_map<ShaderParamName, glm::vec4> Vec4Params;
	std::unordered_map<ShaderParamName, glm::mat4> Mat4Params;
	std::unordered_map<ShaderParamName, glm::mat3> Mat3Params;

	int RenderLayer;
	std::string DebugName;

	void Apply();

	void Set(const std::string& name, const ITexture::sptr& texture);
	void Set(const std::string& name, float value);
	void Set(const std::string& name, const glm::vec2& value);
	void Set(const std::string& name, const glm::vec3& value);
	void Set(const std::string& name, const glm::vec4& value);
	void Set(const std::string& name, const glm::mat4& value);
	void Set(const std::string& name, const glm::mat3& value);

protected:
};
