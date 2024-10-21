#pragma once

#include "RenderCore/Shader.h"

#include <unordered_map>

namespace sl
{

class OpenGLShader : public Shader
{
public:
	OpenGLShader(std::string_view vsSource, std::string_view fsSource);
	OpenGLShader(std::string_view shaderSource, ShaderType type);
	virtual ~OpenGLShader() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void UploadUniform(int location, bool value) override;

	virtual void UploadUniform(int location, int value) override;
	virtual void UploadUniform(int location, const glm::ivec2 &value) override;
	virtual void UploadUniform(int location, const glm::ivec3 &value) override;
	virtual void UploadUniform(int location, const glm::ivec4 &value) override;

	virtual void UploadUniform(int location, unsigned int value) override;
	virtual void UploadUniform(int location, const glm::uvec2 &value) override;
	virtual void UploadUniform(int location, const glm::uvec3 &value) override;
	virtual void UploadUniform(int location, const glm::uvec4 &value) override;

	virtual void UploadUniform(int location, float value) override;
	virtual void UploadUniform(int location, const glm::vec2 &value) override;
	virtual void UploadUniform(int location, const glm::vec3 &value) override;
	virtual void UploadUniform(int location, const glm::vec4 &value) override;

	virtual void UploadUniform(int location, const glm::mat2 &value) override;
	virtual void UploadUniform(int location, const glm::mat3 &value) override;
	virtual void UploadUniform(int location, const glm::mat4 &value) override;

private:
	uint32_t m_programHandle = 0;
};

} // namespace sl
