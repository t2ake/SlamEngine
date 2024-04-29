#include "OpenGLShader.h"

#include "Core/Log.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <vector>

namespace sl
{

namespace
{

static GLenum GetOpenGLShaderType(ShaderType type)
{
	switch (type)
	{
		case ShaderType::VertexShader:
		{
			return GL_VERTEX_SHADER;
		}
		case ShaderType::FragmentShader:
		{
			return GL_FRAGMENT_SHADER;
		}
		case ShaderType::ComputeShader:
		{
			return GL_COMPUTE_SHADER;
		}
		default:
		{
			return GL_INVALID_ENUM;
		}
	}
}

std::string LoadShaderFile(std::string path)
{
	std::ifstream in(path, std::ios::in | std::ios::binary);
	if (!in)
	{
		SL_ENGINE_ERROR("Can't open file : {}", path);
		return {};
	}

	in.seekg(0, std::ios::end);
	std::string src;
	src.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(src.data(), src.size());
	in.close();

	return src;
}

}

OpenGLShader::OpenGLShader(std::string name, std::string vertexPath, std::string fragmentPath) :
	m_shaderProgramName(std::move(name))
{
	SL_ENGINE_TRACE("Compiling standard shader program: \"{}\"", m_shaderProgramName);

	m_programType = ShaderProgramType::Standard;
	if (CompileShader(LoadShaderFile(vertexPath), ShaderType::VertexShader) &&
		CompileShader(LoadShaderFile(fragmentPath), ShaderType::FragmentShader))
	{
		CompileProgram();
	}
}

OpenGLShader::OpenGLShader(std::string name, std::string computePath) :
	m_shaderProgramName(std::move(name))
{
	SL_ENGINE_TRACE("Compiling compute shader program: \"{}\"", m_shaderProgramName);

	m_programType = ShaderProgramType::Compute;
	if (CompileShader(LoadShaderFile(computePath), ShaderType::ComputeShader))
	{
		CompileProgram();
	}
}

OpenGLShader::~OpenGLShader()
{
	glDeleteShader(m_vertexShaderHandle);
	glDeleteShader(m_fragmentShaderHandle);
	glDeleteShader(m_computeShaderHandle);
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

void OpenGLShader::UploadUniform(const std::string &name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void OpenGLShader::UploadUniform(const std::string &name, const glm::ivec2 &value)
{
	glUniform2i(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::UploadUniform(const std::string &name, const glm::ivec3 &value)
{
	glUniform3i(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::UploadUniform(const std::string &name, const glm::ivec4 &value)
{
	glUniform4i(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniform(const std::string &name, unsigned int value)
{
	glUniform1ui(GetUniformLocation(name), value);
}

void OpenGLShader::UploadUniform(const std::string &name, const glm::uvec2 &value)
{
	glUniform2ui(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::UploadUniform(const std::string &name, const glm::uvec3 &value)
{
	glUniform3ui(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::UploadUniform(const std::string &name, const glm::uvec4 &value)
{
	glUniform4ui(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniform(const std::string &name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void OpenGLShader::UploadUniform(const std::string &name, const glm::vec2 &value)
{
	glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::UploadUniform(const std::string &name, const glm::vec3 &value)
{
	glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::UploadUniform(const std::string &name, const glm::vec4 &value)
{
	glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniform(const std::string &name, const glm::mat2 &value)
{
	glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(const std::string &name, const glm::mat3 &value)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(const std::string &name, const glm::mat4 &value)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

bool OpenGLShader::CompileShader(std::string src, ShaderType type)
{
	GLuint shaderHandle = glCreateShader(GetOpenGLShaderType(type));

	const GLchar *source = static_cast<const GLchar *>(src.c_str());
	glShaderSource(shaderHandle, 1, &source, 0);
	glCompileShader(shaderHandle);

#ifndef SL_FINAL
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
		if (ShaderType::FragmentShader == type)
		{
			glDeleteShader(m_vertexShaderHandle);
		}

		return false;
	}
#endif

	if (ShaderType::VertexShader == type)
	{
		m_vertexShaderHandle = shaderHandle;
	}
	else if (ShaderType::FragmentShader == type)
	{
		m_fragmentShaderHandle = shaderHandle;
	}
	else if (ShaderType::ComputeShader == type)
	{
		m_computeShaderHandle = shaderHandle;
	}

	return true;
}

bool OpenGLShader::CompileProgram()
{
	uint32_t programHandle = glCreateProgram();

	if (ShaderProgramType::Compute == m_programType)
	{
		glAttachShader(programHandle, m_computeShaderHandle);
	}
	else
	{
		glAttachShader(programHandle, m_vertexShaderHandle);
		glAttachShader(programHandle, m_fragmentShaderHandle);
	}
	glLinkProgram(programHandle);

#ifndef SL_FINAL
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

		if (ShaderProgramType::Compute == m_programType)
		{
			glDeleteShader(m_computeShaderHandle);
		}
		else
		{
			glDeleteShader(m_vertexShaderHandle);
			glDeleteShader(m_fragmentShaderHandle);
		}
		glDeleteProgram(programHandle);

		return false;
	}
#endif

	if (ShaderProgramType::Compute == m_programType)
	{
		glDetachShader(programHandle, m_computeShaderHandle);
		glDeleteShader(m_computeShaderHandle);
	}
	else
	{
		glDetachShader(programHandle, m_vertexShaderHandle);
		glDetachShader(programHandle, m_fragmentShaderHandle);
		glDeleteShader(m_vertexShaderHandle);
		glDeleteShader(m_fragmentShaderHandle);
	}

	m_programHandle = programHandle;

	return true;
}

int OpenGLShader::GetUniformLocation(const std::string &name)
{
	int location;
	const auto it = m_uniformLocationCache.find(name);
	if (it == m_uniformLocationCache.end())
	{
		location = (int)glGetUniformLocation(m_programHandle, name.c_str());
#ifndef SL_FINEL
		if (-1 == location) { SL_ENGINE_ERROR("Can't find uniform \"{}\" location in {}!", name, m_shaderProgramName); }
#endif
		m_uniformLocationCache[name] = location;
	}
	else
	{
		location = it->second;
	}

	return location;
}

} // namespace sl

static_assert(std::is_same_v<GLint, int>);
