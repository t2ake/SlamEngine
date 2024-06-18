#include "OpenGLShader.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLDefines.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <vector>

namespace sl
{

OpenGLShader::OpenGLShader(uint32_t programHandle) :
	m_programHandle(programHandle)
{

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

void OpenGLShader::UploadUniform(std::string_view name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void OpenGLShader::UploadUniform(std::string_view name, const glm::ivec2 &value)
{
	glUniform2i(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::UploadUniform(std::string_view name, const glm::ivec3 &value)
{
	glUniform3i(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::UploadUniform(std::string_view name, const glm::ivec4 &value)
{
	glUniform4i(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniform(std::string_view name, unsigned int value)
{
	glUniform1ui(GetUniformLocation(name), value);
}

void OpenGLShader::UploadUniform(std::string_view name, const glm::uvec2 &value)
{
	glUniform2ui(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::UploadUniform(std::string_view name, const glm::uvec3 &value)
{
	glUniform3ui(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::UploadUniform(std::string_view name, const glm::uvec4 &value)
{
	glUniform4ui(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniform(std::string_view name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void OpenGLShader::UploadUniform(std::string_view name, const glm::vec2 &value)
{
	glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::UploadUniform(std::string_view name, const glm::vec3 &value)
{
	glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::UploadUniform(std::string_view name, const glm::vec4 &value)
{
	glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniform(std::string_view name, const glm::mat2 &value)
{
	glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(std::string_view name, const glm::mat3 &value)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(std::string_view name, const glm::mat4 &value)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

int OpenGLShader::GetUniformLocation(std::string_view name)
{
	int location;
	const auto &it = m_uniformLocationCache.find(name.data());

	if (it == m_uniformLocationCache.end())
	{
		location = (int)glGetUniformLocation(m_programHandle, name.data());
		if (-1 == location)
		{
			SL_LOG_ERROR("Can not find uniform location: \"{}\"", name);
		}
		m_uniformLocationCache[name.data()] = location;
	}
	else
	{
		location = it->second;
	}

	return location;
}

} // namespace sl

// For OpenGLShader::m_uniformLocationCache
static_assert(std::is_same_v<GLint, int>);
