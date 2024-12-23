#include "ShaderResource.h"

#include "Core/Log.h"
#include "Core/Path.hpp"
#include "Core/Time.hpp"
#include "RenderCore/Shader.h"
#include "Resource/FileIO.hpp"
#include "Resource/ShaderCompiler.h"
#include "Utils/ProfilerCPU.h"

namespace sl
{

namespace
{

bool IsShaderDebugMode()
{
#if !defined(SL_FINAL)
	return true;
#else
	return false;
#endif
}

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

std::string GetShaderBinaryCachePath(std::string_view name, bool isDebugMode)
{
	std::filesystem::path path = Path::FromeRoot("Engine/Binary");
	path /= name;
	if (isDebugMode)
	{
		path += "d";
	}
	path.replace_extension(".bin");

	return path.generic_string();
}

} // namespace

ShaderResource::ShaderResource(std::string_view vsPath, std::string_view fsPath) :
	m_programType(ShaderProgramType::Standard)
{
	m_shaders[0].m_type = ShaderType::VertexShader;
	m_shaders[0].m_debugMode = IsShaderDebugMode();
	m_shaders[0].m_name = Path::NameWithoutExtension(vsPath);
	m_shaders[0].m_assetPath = vsPath;
	m_shaders[0].m_binaryPath = GetShaderBinaryCachePath(m_shaders[0].m_name, m_shaders[0].m_debugMode);

	m_shaders[1].m_type = ShaderType::FragmentShader;
	m_shaders[1].m_debugMode = IsShaderDebugMode();
	m_shaders[1].m_name = Path::NameWithoutExtension(fsPath);
	m_shaders[1].m_assetPath = fsPath;
	m_shaders[1].m_binaryPath = GetShaderBinaryCachePath(m_shaders[1].m_name, m_shaders[1].m_debugMode);

	if (Path::Exists(m_shaders[0].m_binaryPath) && Path::Exists(m_shaders[1].m_binaryPath))
	{
		m_state = ResourceState::Loading;
	}
	else
	{
		m_state = ResourceState::Importing;
	}
}

ShaderResource::ShaderResource(std::string_view path, ShaderProgramType type) :
	m_programType(type)
{
	m_shaders[0].m_type = ProgramTypeToShaderType(m_programType);
	m_shaders[0].m_debugMode = IsShaderDebugMode();
	m_shaders[0].m_name = Path::NameWithExtension(path);
	m_shaders[0].m_assetPath = path;
	m_shaders[0].m_binaryPath = GetShaderBinaryCachePath(m_shaders[0].m_name, m_shaders[0].m_debugMode);

	if (Path::Exists(m_shaders[0].m_binaryPath))
	{
		m_state = ResourceState::Loading;
	}
	else
	{
		m_state = ResourceState::Importing;
	}
}

ShaderResource::~ShaderResource()
{
	OnDestroy();
}

void ShaderResource::OnImport()
{
	SL_PROFILE;

	SL_LOG_TRACE("Loading shader: \"{}\"", m_shaders[0].m_assetPath.c_str());
	m_shaders[0].m_source = FileIO::ReadString(m_shaders[0].m_assetPath);

	if (m_programType == ShaderProgramType::Standard)
	{
		SL_LOG_TRACE("Loading shader: \"{}\"", m_shaders[1].m_assetPath.c_str());
		m_shaders[1].m_source = FileIO::ReadString(m_shaders[1].m_assetPath);
	}

#if !defined(SL_FINAL)
	if (m_shaders[0].m_source.empty() ||
		(m_programType == ShaderProgramType::Standard && m_shaders[1].m_source.empty()))
	{
		SL_LOG_ERROR("Failed to import shader!");
		m_state = ResourceState::Destroying;
		return;
	}
#endif

	m_state = ResourceState::Building;
}

void ShaderResource::OnBuild()
{
	SL_PROFILE;

	SL_LOG_TRACE("Compiling SPIR-V: \"{}\"", m_shaders[0].m_name.c_str());
	
	Timer timer;
	auto spirvData = ShaderCompiler::SourceToSpirv(m_shaders[0]);
	FileIO::WriteBinary(m_shaders[0].m_binaryPath,
		reinterpret_cast<char *>(spirvData.data()), spirvData.size() * sizeof(uint32_t));
	m_shaders[0].m_source = ShaderCompiler::SpirvToSource(std::move(spirvData));
	SL_LOG_TRACE("  Done in {} ms", timer.GetDuration());

	if (m_programType == ShaderProgramType::Standard)
	{
		SL_LOG_TRACE("Compiling SPIR-V: \"{}\"", m_shaders[1].m_name.c_str());

		timer.Reset();
		auto fragSpirvData = ShaderCompiler::SourceToSpirv(m_shaders[1]);
		FileIO::WriteBinary(m_shaders[1].m_binaryPath,
			reinterpret_cast<char *>(fragSpirvData.data()), fragSpirvData.size() * sizeof(uint32_t));
		m_shaders[1].m_source = ShaderCompiler::SpirvToSource(std::move(fragSpirvData));
		SL_LOG_TRACE("  Done in {} ms", timer.GetDuration());
	}

#if !defined(SL_FINAL)
	if (m_shaders[0].m_source.empty() ||
		(m_programType == ShaderProgramType::Standard && m_shaders[1].m_source.empty()))
	{
		SL_LOG_ERROR("Failed to build shader!");
		m_state = ResourceState::Destroying;
		return;
	}
#endif

	m_state = ResourceState::Uploading;
}

void ShaderResource::OnLoad()
{
	SL_PROFILE;

	SL_LOG_TRACE("Loading SPIR-V cache: \"{}\"", m_shaders[0].m_binaryPath.c_str());
	auto spirvBinary = FileIO::ReadBinary<uint32_t>(m_shaders[0].m_binaryPath);
	m_shaders[0].m_source = ShaderCompiler::SpirvToSource(std::move(spirvBinary));

	if (m_programType == ShaderProgramType::Standard)
	{
		SL_LOG_TRACE("Loading SPIR-V cache: \"{}\"", m_shaders[1].m_binaryPath.c_str());
		auto fragSpirvBinary = FileIO::ReadBinary<uint32_t>(m_shaders[1].m_binaryPath);
		m_shaders[1].m_source = ShaderCompiler::SpirvToSource(std::move(fragSpirvBinary));
	}

#if !defined(SL_FINAL)
	if (m_shaders[0].m_source.empty() ||
		(m_programType == ShaderProgramType::Standard && m_shaders[1].m_source.empty()))
	{
		SL_LOG_ERROR("Failed to load shader binary cache!");
		m_state = ResourceState::Destroying;
		return;
	}
#endif

	m_state = ResourceState::Uploading;
}

void ShaderResource::OnUpload()
{
	SL_PROFILE;

	// We assume that a shader is named by its shader program name plus the stage suffix.
	// - Shader Program: XXX
	// - Vertex Shader: XXX_vert.glsl
	// - Fragment Shader: XXX_frag.glsl
	SL_LOG_TRACE("Uploading shader program: \"{}\"", m_shaders[0].m_name.substr(0, m_shaders[0].m_name.rfind('_')));

	if (m_programType == ShaderProgramType::Standard)
	{
		m_pShaderProgram.reset(Shader::Create(m_shaders[0].m_source, m_shaders[1].m_source));
	}
	else
	{
		m_pShaderProgram.reset(Shader::Create(m_shaders[0].m_source, m_shaders[0].m_type));
	}

#if !defined(SL_FINAL)
	if (!m_pShaderProgram)
	{
		SL_LOG_ERROR("Failed to create shader GPU handle!");
		m_state = ResourceState::Destroying;
		return;
	}
#endif

	m_state = ResourceState::Ready;
}

void ShaderResource::OnReady()
{
	if (m_destroyDelay <= 60 && m_destroyDelay++ == 60)
	{
		DestroyCPUData();
	}
}

void ShaderResource::OnDestroy()
{
	SL_PROFILE;

	DestroyCPUData();
	m_pShaderProgram.reset();

	m_state = ResourceState::Destroyed;
}

void ShaderResource::DestroyCPUData()
{
	SL_PROFILE;

	m_shaders[0].m_source.clear();
	std::string().swap(m_shaders[0].m_source);

	if (m_programType == ShaderProgramType::Standard)
	{
		m_shaders[1].m_source.clear();
		std::string().swap(m_shaders[1].m_source);
	}
}

} // namespace sl
