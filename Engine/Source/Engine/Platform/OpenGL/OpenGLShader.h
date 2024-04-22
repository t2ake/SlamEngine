#pragma once

#include "RenderCore/Shader.h"

#include <cstdint>

namespace sl
{

class OpenGLShader : public Shader
{

public:
	OpenGLShader(std::string vertexSrc, std::string fragmentSrc);
	virtual ~OpenGLShader() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void UploadUniformMat4(std::string name, glm::mat4 mat) const override;

private:
	bool CompileShader(std::string src, uint32_t shaderType);
	bool CompileProgram();

	// TODO: Set a "InvalidHandle" as 0
	uint32_t m_vertexShaderHandle = 0;
	uint32_t m_fragmentShaderHandle = 0;
	uint32_t m_programHandle = 0;
};

} // namespace sl
