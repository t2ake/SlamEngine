#include "OpenGLShader.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLDefines.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace sl
{

namespace
{

uint32_t UploadShader(const char *pSource, size_t size, ShaderType type)
{
	static_assert(std::is_same_v<GLchar, char>);
	const GLchar *pGLSource = static_cast<const GLchar *>(pSource);
	const GLint GLsize = (GLint)size;

	GLuint shaderHandle = glCreateShader(GLShaderType[(size_t)type]);
	glShaderSource(shaderHandle, 1, &pGLSource, &GLsize);
	glCompileShader(shaderHandle);

#if !defined(SL_FINAL)
	GLint isCompiled = 0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
	if (GL_FALSE == isCompiled)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, infoLog.data());
		SL_LOG_ERROR("Shader upload failed: {}", infoLog.data());

		glDeleteShader(shaderHandle);

		return 0;
	}
#endif

	return shaderHandle;
}

uint32_t UploadProgram(uint32_t vsHandle, uint32_t fsHandle = 0)
{
	uint32_t programHandle = glCreateProgram();
	glAttachShader(programHandle, vsHandle);
	if (0 != fsHandle)
	{
		glAttachShader(programHandle, fsHandle);
	}
	glLinkProgram(programHandle);

#if !defined(SL_FINAL)
	GLint isLinked = 0;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programHandle, maxLength, &maxLength, infoLog.data());
		SL_LOG_ERROR("Shader program upload failed: {}", infoLog.data());

		glDeleteProgram(programHandle);
		glDeleteShader(vsHandle);
		if (0 != fsHandle)
		{
			glDeleteShader(fsHandle);
		}

		return 0;
	}
#endif

	glDetachShader(programHandle, vsHandle);
	glDeleteShader(vsHandle);

	if (0 != fsHandle)
	{
		glDetachShader(programHandle, fsHandle);
		glDeleteShader(fsHandle);
	}

	return programHandle;
}

} // namespace

OpenGLShader::OpenGLShader(std::string_view vsSource, std::string_view fsSource)
{
	uint32_t vsHandle = UploadShader(vsSource.data(), vsSource.size(), ShaderType::VertexShader);
	uint32_t fsHandle = UploadShader(fsSource.data(), fsSource.size(), ShaderType::FragmentShader);
	m_programHandle = UploadProgram(vsHandle, fsHandle);
}

OpenGLShader::OpenGLShader(std::string_view shaderSource, ShaderType type)
{
	uint32_t shaderHandle = UploadShader(shaderSource.data(), shaderSource.size(), type);
	m_programHandle = UploadProgram(shaderHandle);
}

OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(m_programHandle);
}

void OpenGLShader::Bind() const
{
	glUseProgram(m_programHandle);
}

void OpenGLShader::Unbind() const
{
	glUseProgram(0);
}

void OpenGLShader::UploadUniform(int location, int value)
{
	glUniform1i(location, value);
}

void OpenGLShader::UploadUniform(int location, const glm::ivec2 &value)
{
	glUniform2i(location, value.x, value.y);
}

void OpenGLShader::UploadUniform(int location, const glm::ivec3 &value)
{
	glUniform3i(location, value.x, value.y, value.z);
}

void OpenGLShader::UploadUniform(int location, const glm::ivec4 &value)
{
	glUniform4i(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniform(int location, unsigned int value)
{
	glUniform1ui(location, value);
}

void OpenGLShader::UploadUniform(int location, const glm::uvec2 &value)
{
	glUniform2ui(location, value.x, value.y);
}

void OpenGLShader::UploadUniform(int location, const glm::uvec3 &value)
{
	glUniform3ui(location, value.x, value.y, value.z);
}

void OpenGLShader::UploadUniform(int location, const glm::uvec4 &value)
{
	glUniform4ui(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniform(int location, float value)
{
	glUniform1f(location, value);
}

void OpenGLShader::UploadUniform(int location, const glm::vec2 &value)
{
	glUniform2f(location, value.x, value.y);
}

void OpenGLShader::UploadUniform(int location, const glm::vec3 &value)
{
	glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::UploadUniform(int location, const glm::vec4 &value)
{
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniform(int location, const glm::mat2 &value)
{
	glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(int location, const glm::mat3 &value)
{
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(int location, const glm::mat4 &value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

} // namespace sl

static_assert(std::is_same_v<GLint, int>);
