#pragma once

#include "RenderCore/Shader.h"

#include <unordered_map>

namespace sl
{

class OpenGLShader : public Shader
{

public:
	OpenGLShader(std::string name, std::string vertexSrc, std::string fragmentSrc);
	OpenGLShader(std::string name, std::string computeSrc);
	virtual ~OpenGLShader() override;

	void SetName(std::string name) { m_shaderProgramName = std::move(name); }
	std::string &GetName() { return m_shaderProgramName; }
	const std::string &GetName() const { return m_shaderProgramName; }

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void UploadUniform(const std::string &name, int value) override;
	virtual void UploadUniform(const std::string &name, const glm::ivec2 &value) override;
	virtual void UploadUniform(const std::string &name, const glm::ivec3 &value) override;
	virtual void UploadUniform(const std::string &name, const glm::ivec4 &value) override;

	virtual void UploadUniform(const std::string &name, unsigned int value) override;
	virtual void UploadUniform(const std::string &name, const glm::uvec2 &value) override;
	virtual void UploadUniform(const std::string &name, const glm::uvec3 &value) override;
	virtual void UploadUniform(const std::string &name, const glm::uvec4 &value) override;

	virtual void UploadUniform(const std::string &name, float value) override;
	virtual void UploadUniform(const std::string &name, const glm::vec2 &value) override;
	virtual void UploadUniform(const std::string &name, const glm::vec3 &value) override;
	virtual void UploadUniform(const std::string &name, const glm::vec4 &value) override;

	virtual void UploadUniform(const std::string &name, const glm::mat2 &value) override;
	virtual void UploadUniform(const std::string &name, const glm::mat3 &value) override;
	virtual void UploadUniform(const std::string &name, const glm::mat4 &value) override;

private:
	bool CompileShader(std::string src, ShaderType type);
	bool CompileProgram();

	int GetUniformLocation(const std::string &name);

	std::string m_shaderProgramName = "Default Shader Name";
	ShaderProgramType m_programType = ShaderProgramType::Standard;

	uint32_t m_vertexShaderHandle = 0;
	uint32_t m_fragmentShaderHandle = 0;
	uint32_t m_computeShaderHandle = 0;
	uint32_t m_programHandle = 0;

	std::unordered_map<std::string, uint32_t> m_uniformLocationCache;
};

} // namespace sl
