#include "ShaderResource.h"

#include "Core/Log.h"
#include "RenderCore/Shader.h"
#include "Resource/ResourceLoader.h"

// TEMPLATE: Replace these by SPRI-V
#include "Platform/OpenGL/OpenGLDefines.h"
#include <glad/glad.h>

namespace sl
{

namespace
{

ShaderType ProgramTypeToShaderType(ShaderProgramType programType)
{
	switch (programType)
	{
		case sl::ShaderProgramType::Compute:
		{
			return ShaderType::ComputeShader;
			break;
		}
		default:
		{
			SL_ENGINE_ASSERT(false);
			return ShaderType::VertexShader;
			break;
		}
	}
}

uint32_t CompileShader(ShaderType type, const void *src, size_t size)
{
	GLuint shaderHandle = glCreateShader(GLShaderType[(size_t)type]);

	const GLchar* source = static_cast<const GLchar*>(src);
	const GLint GLsize = (GLint)size;
	glShaderSource(shaderHandle, 1, &source, &GLsize);
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

		return 0;
	}
#endif

	return shaderHandle;
}

uint32_t CompileProgram(uint32_t vsHandle, uint32_t fsHandle)
{
	uint32_t programHandle = glCreateProgram();
	glAttachShader(programHandle, vsHandle);
	glAttachShader(programHandle, fsHandle);
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

		glDeleteProgram(programHandle);
		glDeleteShader(vsHandle);
		glDeleteShader(fsHandle);

		return 0;
	}
#endif

	glDetachShader(programHandle, vsHandle);
	glDetachShader(programHandle, fsHandle);
	glDeleteShader(vsHandle);
	glDeleteShader(fsHandle);

	return programHandle;
}

uint32_t CompileProgram(uint32_t handle)
{
	uint32_t programHandle = glCreateProgram();
	glAttachShader(programHandle, handle);
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

		glDeleteProgram(programHandle);
		glDeleteShader(handle);

		return 0;
	}
#endif

	glDetachShader(programHandle, handle);
	glDeleteShader(handle);

	return programHandle;
}

} // namespace

ShaderResource::ShaderResource(std::string_view vsPath, std::string_view fsPath) :
	m_programType(ShaderProgramType::Standard)
{
	m_shaders[0].m_type = ShaderType::VertexShader;
	m_shaders[0].m_assetPath = vsPath;
	m_shaders[1].m_type = ShaderType::FragmentShader;
	m_shaders[1].m_assetPath = fsPath;

	SetStatus(ResourceStatus::Importing);
}

ShaderResource::ShaderResource(ShaderProgramType type, std::string_view path) :
	m_programType(type)
{
	m_shaders[0].m_type = ProgramTypeToShaderType(m_programType);
	m_shaders[0].m_assetPath = path;

	SetStatus(ResourceStatus::Importing);
}

ShaderResource::~ShaderResource()
{
	OnDestroy();
}

void ShaderResource::OnImport()
{
	m_shaders[0].m_rowData = ResourceLoader::LoadFile(m_shaders[0].m_assetPath);

	if (ShaderProgramType::Standard == m_programType)
	{
		m_shaders[1].m_rowData = ResourceLoader::LoadFile(m_shaders[1].m_assetPath);
	}

	SetStatus(ResourceStatus::Building);
}

void ShaderResource::OnBuild()
{
	SetStatus(ResourceStatus::Uploading);
}

void ShaderResource::OnLoad()
{
	SetStatus(ResourceStatus::Uploading);
}

void ShaderResource::OnUpload()
{
	const auto& shader0 = m_shaders[0];
	uint32_t shaderHandle = CompileShader(shader0.m_type, shader0.m_rowData.data(), shader0.m_rowData.size());

	if (0 == shaderHandle)
	{
		SetStatus(ResourceStatus::Destroying);
		return;
	}

	uint32_t programHandle = 0;
	if (ShaderProgramType::Standard == m_programType)
	{
		const auto& fragmentShader = m_shaders[1];
		uint32_t fsHandle = CompileShader(fragmentShader.m_type, fragmentShader.m_rowData.data(), fragmentShader.m_rowData.size());

		if (0 == fsHandle)
		{
			SetStatus(ResourceStatus::Destroying);
			return;
		}

		programHandle = CompileProgram(shaderHandle, fsHandle);
	}
	else
	{
		programHandle = CompileProgram(shaderHandle);
	}

	if (0 == programHandle)
	{
		SetStatus(ResourceStatus::Destroying);
		return;
	}

	m_pShaderProgram.reset(Shader::Creat(programHandle));

	SetStatus(ResourceStatus::Ready);
}

void ShaderResource::OnReady()
{
	static uint32_t frameCount = 0;
	if (frameCount < 60)
	{
		++frameCount;
	}
	else if (frameCount == 60)
	{
		DestroyCPUData();
	}
}

void ShaderResource::OnDestroy()
{
	DestroyCPUData();
	m_pShaderProgram.reset();

	SetStatus(ResourceStatus::Destroyed);
}

void ShaderResource::DestroyCPUData()
{
	m_shaders[0].m_rowData.clear();
	std::vector<std::byte>().swap(m_shaders[0].m_rowData);

	m_shaders[1].m_rowData.clear();
	std::vector<std::byte>().swap(m_shaders[1].m_rowData);
}

} // namespace sl
