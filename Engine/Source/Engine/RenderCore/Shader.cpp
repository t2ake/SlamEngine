#include "Shader.h"

#include "Log/Log.h"

#include <glad/glad.h>

#include <vector>

namespace sl
{

Shader::Shader(std::string vertexSrc, std::string fragmentSrc)
{
	// TODO: sl::ShaderType, sl::ShaderProgramType

	SL_ENGINE_TRACE("Compiling shader program");
	if (CompileShader(std::move(vertexSrc), GL_VERTEX_SHADER) &&
		CompileShader(std::move(fragmentSrc), GL_FRAGMENT_SHADER))
	{
		CompileProgram();
	}
}

Shader::~Shader()
{
	glDeleteProgram(m_programHandle);
}

void Shader::Bind()
{
	glUseProgram(m_programHandle);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

bool Shader::CompileShader(std::string src, uint32_t shaderType)
{
	GLuint shaderHandle = glCreateShader(shaderType);

	const GLchar *source = static_cast<const GLchar *>(src.c_str());
	glShaderSource(shaderHandle, 1, &source, 0);
	glCompileShader(shaderHandle);

	GLint isCompiled = 0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
	if (GL_FALSE == isCompiled)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, infoLog.data());
		SL_ENGINE_ERROR("Shader compile failed: {}", infoLog.data());

		glDeleteShader(shaderHandle);
		if (GL_FRAGMENT_SHADER == shaderType)
		{
			glDeleteShader(m_vertexShaderHandle);
		}

		return false;
	}

	if (GL_VERTEX_SHADER == shaderType)
	{
		m_vertexShaderHandle = shaderHandle;
	}
	else if (GL_FRAGMENT_SHADER == shaderType)
	{
		m_fragmentShaderHandle = shaderHandle;
	}

	return true;
}

bool Shader::CompileProgram()
{
	uint32_t programHandle = glCreateProgram();

	glAttachShader(programHandle, m_vertexShaderHandle);
	glAttachShader(programHandle, m_fragmentShaderHandle);
	glLinkProgram(programHandle);

	GLint isLinked = 0;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programHandle, maxLength, &maxLength, infoLog.data());
		SL_ENGINE_ERROR("Shader program compile failed: {}", infoLog.data());

		glDeleteShader(m_vertexShaderHandle);
		glDeleteShader(m_fragmentShaderHandle);
		glDeleteProgram(programHandle);

		return false;
	}

	glDetachShader(programHandle, m_vertexShaderHandle);
	glDetachShader(programHandle, m_fragmentShaderHandle);
	m_programHandle = programHandle;

	return true;
}

} // namespace sl
