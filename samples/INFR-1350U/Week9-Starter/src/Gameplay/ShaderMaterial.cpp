#include "ShaderMaterial.h"

template<typename T>
void SubmitUniforms(const Shader::sptr& shader, const std::unordered_map<ShaderParamName, T>& values) {
	for (auto& kvp : values) {
		shader->SetUniform(kvp.first.Location, kvp.second);
	}
}

template<typename T>
void SubmitUniformsMat(const Shader::sptr& shader, const std::unordered_map<ShaderParamName, T>& values) {
	for (auto& kvp : values) {
		shader->SetUniformMatrix(kvp.first.Location, kvp.second);
	}
}

ShaderMaterial::ShaderMaterial()
	: Shader(nullptr),  RenderLayer(0)
{
}

ShaderMaterial::~ShaderMaterial() {
	LOG_INFO("Deleting material");
}

void ShaderMaterial::Apply()
{	
	int slot = 1;
	for (auto& kvp : Textures) {
		if (kvp.first.Location != -1 && kvp.second != nullptr) {
			Shader->SetUniform(kvp.first.Location, slot);
			kvp.second->Bind(slot);
			slot++;
		}
	}

	SubmitUniforms(Shader, FloatParams);
	SubmitUniforms(Shader, Vec2Params);
	SubmitUniforms(Shader, Vec3Params);
	SubmitUniforms(Shader, Vec4Params);
	SubmitUniformsMat(Shader, Mat4Params);
	SubmitUniformsMat(Shader, Mat3Params);
}

void ShaderMaterial::Set(const std::string& name, const ITexture::sptr& texture) {
	LOG_ASSERT(Shader != nullptr, "Must set Material shader before setting params");
	ShaderParamName pName = name;
	pName.Location = Shader->GetUniformLocation(name);
	Textures[pName] = texture;
}

void ShaderMaterial::Set(const std::string& name, float value) {
	LOG_ASSERT(Shader != nullptr, "Must set Material shader before setting params");
	ShaderParamName pName = name;
	pName.Location = Shader->GetUniformLocation(name);
	FloatParams[pName] = value;
}

void ShaderMaterial::Set(const std::string& name, const glm::vec2& value) {
	LOG_ASSERT(Shader != nullptr, "Must set Material shader before setting params");
	ShaderParamName pName = name;
	pName.Location = Shader->GetUniformLocation(name);
	Vec2Params[pName] = value;
}

void ShaderMaterial::Set(const std::string& name, const glm::vec3& value) {
	LOG_ASSERT(Shader != nullptr, "Must set Material shader before setting params");
	ShaderParamName pName = name;
	pName.Location = Shader->GetUniformLocation(name);
	Vec3Params[pName] = value;
}

void ShaderMaterial::Set(const std::string& name, const glm::vec4& value) {
	LOG_ASSERT(Shader != nullptr, "Must set Material shader before setting params");
	ShaderParamName pName = name;
	pName.Location = Shader->GetUniformLocation(name);
	Vec4Params[pName] = value;
}

void ShaderMaterial::Set(const std::string& name, const glm::mat4& value) {
	LOG_ASSERT(Shader != nullptr, "Must set Material shader before setting params");
	ShaderParamName pName = name;
	pName.Location = Shader->GetUniformLocation(name);
	Mat4Params[pName] = value;
}

void ShaderMaterial::Set(const std::string& name, const glm::mat3& value) {
	LOG_ASSERT(Shader != nullptr, "Must set Material shader before setting params");
	ShaderParamName pName = name;
	pName.Location = Shader->GetUniformLocation(name);
	Mat3Params[pName] = value;
}
