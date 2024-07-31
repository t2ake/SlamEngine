#include "ShaderCompiler.h"

#include "Core/EnumOf.hpp"
#include "Core/Path.hpp"
#include "RenderCore/RenderCore.h"
#include "Resource/FileIO.h"

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
		m_pContainer = new ShaderIncluderContainer;
		auto &container = *m_pContainer;

		// Include path with "../" is not supported for now.
		if (type == shaderc_include_type_relative)
		{
			container[0] = Path::FromeAsset("Shader/");
		}
		else if(type == shaderc_include_type_standard)
		{
			container[0] = Path::FromeAsset("Shader/Header/");
		}
		else
		{
			SL_ASSERT(false, "Shaderc unknown include type!");
		}
		container[0] += requested_source;

		container[1] = FileIO::ReadString(container[0]);

		auto *pResult = new shaderc_include_result;
		pResult->source_name = container[0].c_str();
		pResult->source_name_length = container[0].size();
		pResult->content = container[1].c_str();
		pResult->content_length = container[1].size();

		return pResult;
	}

	virtual void ReleaseInclude(shaderc_include_result *pResult) override
	{
		delete m_pContainer;
		delete pResult;
	}

private:
	// [0] is the absolute path of the file, [1] is the content of the file.
	using ShaderIncluderContainer = std::array<std::string, 2>;

	// To ensure requested data is valid before calling ReleaseInclude.
	ShaderIncluderContainer *m_pContainer = nullptr;
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
			SL_LOG_ERROR("Shader preprocess failed: \"{}\"", name);
			SL_LOG_ERROR(result.GetErrorMessage());
			return {};
		}

		preprocessedShaderSource = { result.cbegin(), result.cend() };
	}

	// 2. Compile to SPIR-V
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

#if defined(SL_DEBUG)
		options.SetGenerateDebugInfo();
		options.SetOptimizationLevel(shaderc_optimization_level_zero);
#else
		options.SetOptimizationLevel(shaderc_optimization_level_performance);
#endif

		// TODO: Start from vulkan glsl in the future.
		options.SetSourceLanguage(shaderc_source_language_glsl);
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

		shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
			preprocessedShaderSource.c_str(), preprocessedShaderSource.size(),
			shaderKind, name, "main", options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			SL_LOG_ERROR("Compile shader to SPIR-V failed: \"{}\"", name);
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
