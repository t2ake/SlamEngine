#pragma once

#include <cstdint>
#include <string>

namespace sl
{

// TODO: Shader resource
class Shader final
{
public:
	Shader(std::string vertexSrc, std::string fragmentSrc);
	~Shader();

	void Bind();
	void Unbind();

private:
	bool CompileShader(std::string src, uint32_t shaderType);
	bool CompileProgram();

	// TODO: Set a "InvalidHandle" as uint32_t::max, or 0?
	uint32_t m_vertexShaderHandle = 0;
	uint32_t m_fragmentShaderHandle = 0;
	uint32_t m_programHandle = 0;
};

} // namespace sl
