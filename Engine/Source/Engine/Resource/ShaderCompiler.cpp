#include "ShaderCompiler.h"

#include "Core/Path.hpp"
#include "RenderCore/RenderCore.h"
#include "Resource/FileIO.hpp"
#include "Utils/EnumOf.hpp"

#include <shaderc/shaderc.hpp>
#include <spirv_glsl.hpp>

namespace sl
{

namespace
{

constexpr const char *BackendToDefinition[nameof::enum_count<GraphicsBackend>()] =
{
	"SL_NONE",	    // GraphicsBackend::None
	"SL_OPENGL",    // GraphicsBackend::OpenGL,
	"SL_VULKAN",    // GraphicsBackend::Vulkan,
	"SL_DIRECTX",   // GraphicsBackend::DirectX11,
	"SL_DIRECTX",   // GraphicsBackend::DirectX12,
	"SL_METAL",	    // GraphicsBackend::Metal,
};

constexpr shaderc_shader_kind ShaderTypeToShaderKind[nameof::enum_count<ShaderType>()] =
{
	shaderc_vertex_shader,	 // ShaderType::VertexShader
	shaderc_fragment_shader, // ShaderType::FragmentShader
	shaderc_compute_shader,	 // ShaderType::ComputeShader
};

class ShaderIncluder : public shaderc::CompileOptions::IncluderInterface
{
public:
	virtual shaderc_include_result *GetInclude(
		const char *requested_source,
		shaderc_include_type type,
		const char *requesting_source,
		size_t include_depth) override
	{
		std::filesystem::path path;
		if (type == shaderc_include_type_relative)
		{
			path = Path::FromeAsset("Shader");
		}
		else if(type == shaderc_include_type_standard)
		{
			path = Path::FromeAsset("Shader/Lib");
		}
		else
		{
			SL_ASSERT(false, "Shaderc unknown include type!");
		}

		path /= requested_source;
		m_pathContainer = path.generic_string();
		m_contentContainer = FileIO::ReadString(m_pathContainer);

		auto *pResult = new shaderc_include_result;
		pResult->source_name = m_pathContainer.c_str();
		pResult->source_name_length = m_pathContainer.size();
		pResult->content = m_contentContainer.c_str();
		pResult->content_length = m_contentContainer.size();

		return pResult;
	}

	virtual void ReleaseInclude(shaderc_include_result *pResult) override
	{
		delete pResult;
	}

private:
	// To ensure requested data is valid before calling ReleaseInclude.
	std::string m_pathContainer;
	std::string m_contentContainer;
};

} // namespace

std::vector<uint32_t> ShaderCompiler::SourceToSpirv(const ShaderInfo &info)
{
	const char *name = info.m_name.c_str();
	shaderc_shader_kind shaderKind = ShaderTypeToShaderKind[(size_t)info.m_type];
	std::string preprocessedShaderSource;

	// 1. Preprocess
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		// Include
		options.SetIncluder(std::make_unique<ShaderIncluder>());

		// Definition
		options.AddMacroDefinition(BackendToDefinition[(size_t)RenderCore::GetBackend()]);

		shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(
			info.m_source.c_str(), info.m_source.size(),
			shaderKind, name, options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			SL_LOG_ERROR("Failed to preprocess shader!");
			SL_LOG_ERROR(result.GetErrorMessage());
			return {};
		}

		preprocessedShaderSource = { result.cbegin(), result.cend() };
	}

	// 2. Compile to SPIR-V
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		if (info.m_debugMode)
		{
			options.SetGenerateDebugInfo();
			options.SetOptimizationLevel(shaderc_optimization_level_zero);
		}
		else
		{
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		// TODO: Start from vulkan glsl in the future.
		options.SetSourceLanguage(shaderc_source_language_glsl);
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

		shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
			preprocessedShaderSource.c_str(), preprocessedShaderSource.size(),
			shaderKind, name, "main", options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			SL_LOG_ERROR("Failed to compile shader to SPIR-V!");
			SL_LOG_ERROR(result.GetErrorMessage());
			return {};
		}

		return { result.cbegin(), result.cend() };
	}

	return {};
}

std::string ShaderCompiler::SpirvToSource(std::vector<uint32_t> spirv)
{
	spirv_cross::CompilerGLSL::Options options;
	options.version = 450;

	spirv_cross::CompilerGLSL glsl(std::move(spirv));
	glsl.set_common_options(options);

	return glsl.compile();
}

void ShaderCompiler::ReflectSpirv(const std::vector<uint32_t> &spirv)
{
	spirv_cross::CompilerGLSL glsl(spirv);
	spirv_cross::ShaderResources resources = glsl.get_shader_resources();
}

} // namespace sl
