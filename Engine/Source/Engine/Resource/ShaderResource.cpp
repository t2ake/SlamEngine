#include "ShaderResource.h"

#include "Core/Log.h"
#include "Core/Path.hpp"
#include "RenderCore/Shader.h"
#include "Resource/FileIO.h"
#include "Resource/ShaderCompiler.h"

namespace sl
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
			SL_ASSERT(false);
			return ShaderType::VertexShader;
			break;
		}
	}
}

ShaderResource::ShaderResource(std::string_view vsPath, std::string_view fsPath) :
	m_programType(ShaderProgramType::Standard)
{
	m_shaders[0].m_type = ShaderType::VertexShader;
	m_shaders[0].m_name = Path::NameWithExtension(vsPath);
	m_shaders[0].m_assetPath = vsPath;

	m_shaders[1].m_type = ShaderType::FragmentShader;
	m_shaders[1].m_name = Path::NameWithExtension(fsPath);
	m_shaders[1].m_assetPath = fsPath;

	SetStatus(ResourceStatus::Importing);
}

ShaderResource::ShaderResource(std::string_view path, ShaderProgramType type) :
	m_programType(type)
{
	m_shaders[0].m_type = ProgramTypeToShaderType(m_programType);
	m_shaders[0].m_name = Path::NameWithExtension(path);
	m_shaders[0].m_assetPath = path;

	SetStatus(ResourceStatus::Importing);
}

ShaderResource::~ShaderResource()
{
	OnDestroy();
}

void ShaderResource::OnImport()
{
	SL_LOG_TRACE("Loading shader: \"{}\"", m_shaders[0].m_assetPath.c_str());
	m_shaders[0].m_rowData = FileIO::LoadString(m_shaders[0].m_assetPath);

	if (ShaderProgramType::Standard == m_programType)
	{
		SL_LOG_TRACE("Loading shader: \"{}\"", m_shaders[1].m_assetPath.c_str());
		m_shaders[1].m_rowData = FileIO::LoadString(m_shaders[1].m_assetPath);
	}

#ifndef SL_FINAL
	if (m_shaders[0].m_rowData.empty() || m_shaders[1].m_rowData.empty())
	{
		SetStatus(ResourceStatus::Destroying);
		return;
	}
#endif

	SetStatus(ResourceStatus::Building);
}

void ShaderResource::OnBuild()
{
	SL_LOG_TRACE("Compiling SPIR-V: \"{}\"", m_shaders[0].m_name.c_str());
	m_shaders[0].m_rowData = ShaderCompiler::CompileShader(m_shaders[0]);

	if (ShaderProgramType::Standard == m_programType)
	{
		SL_LOG_TRACE("Compiling SPIR-V: \"{}\"", m_shaders[1].m_name.c_str());
		m_shaders[1].m_rowData = ShaderCompiler::CompileShader(m_shaders[1]);
	}

#ifndef SL_FINAL
	if (m_shaders[0].m_rowData.empty() || m_shaders[1].m_rowData.empty())
	{
		SetStatus(ResourceStatus::Destroying);
		return;
	}
#endif

	SetStatus(ResourceStatus::Uploading);
}

void ShaderResource::OnLoad()
{
	SetStatus(ResourceStatus::Uploading);
}

void ShaderResource::OnUpload()
{
	if (ShaderProgramType::Standard == m_programType)
	{
		SL_LOG_TRACE("Uploading shader program: \"{}\"", Path::NameWithoutExtension(m_shaders[0].m_name));
		m_pShaderProgram.reset(Shader::Create(m_shaders[0].m_rowData, m_shaders[1].m_rowData));
	}
	else
	{
		SL_LOG_TRACE("Uploading shader program: \"{}\"", Path::NameWithoutExtension(m_shaders[0].m_name));
		m_pShaderProgram.reset(Shader::Create(m_shaders[0].m_rowData, m_shaders[0].m_type));
	}

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
	std::string().swap(m_shaders[0].m_rowData);

	if (ShaderProgramType::Standard == m_programType)
	{
		m_shaders[1].m_rowData.clear();
		std::string().swap(m_shaders[1].m_rowData);
	}
}

} // namespace sl
