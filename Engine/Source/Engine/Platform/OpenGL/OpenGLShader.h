#pragma once

#include "RenderCore/Shader.h"

#include <unordered_map>

namespace sl
{

class OpenGLShader : public Shader
{
public:
	OpenGLShader(uint32_t programHandle);
	virtual ~OpenGLShader() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void UploadUniform(std::string_view name, int value) override;
	virtual void UploadUniform(std::string_view name, const glm::ivec2 &value) override;
	virtual void UploadUniform(std::string_view name, const glm::ivec3 &value) override;
	virtual void UploadUniform(std::string_view name, const glm::ivec4 &value) override;

	virtual void UploadUniform(std::string_view name, unsigned int value) override;
	virtual void UploadUniform(std::string_view name, const glm::uvec2 &value) override;
	virtual void UploadUniform(std::string_view name, const glm::uvec3 &value) override;
	virtual void UploadUniform(std::string_view name, const glm::uvec4 &value) override;

	virtual void UploadUniform(std::string_view name, float value) override;
	virtual void UploadUniform(std::string_view name, const glm::vec2 &value) override;
	virtual void UploadUniform(std::string_view name, const glm::vec3 &value) override;
	virtual void UploadUniform(std::string_view name, const glm::vec4 &value) override;

	virtual void UploadUniform(std::string_view name, const glm::mat2 &value) override;
	virtual void UploadUniform(std::string_view name, const glm::mat3 &value) override;
	virtual void UploadUniform(std::string_view name, const glm::mat4 &value) override;

private:
	int GetUniformLocation(std::string_view name);

	uint32_t m_programHandle;
	std::unordered_map<std::string, int> m_uniformLocationCache;
};

} // namespace sl
