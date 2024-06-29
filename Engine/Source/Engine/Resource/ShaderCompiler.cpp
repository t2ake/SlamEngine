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
	"SL_DIRECTX11", // GraphicsBackend::DirectX11,
	"SL_DIRECTX12", // GraphicsBackend::DirectX12,
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

		// Include with "../" is not supported for now.
		container[0] = Path::FromeAsset("Shader/") + requested_source;
		container[1] = FileIO::LoadString(container[0]);

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
	using ShaderIncluderContainer = std::array<std::string, 2>;

	// To ensure requested data is valid before calling ReleaseInclude.
	ShaderIncluderContainer *m_pContainer = nullptr;
};

} // namespace

std::string ShaderCompiler::CompileShader(const ShaderInfo &info)
{
	const char *name = info.m_name.c_str();
	shaderc_shader_kind shaderKind = ShaderTypeToShaderKind[(size_t)info.m_type];
	
	std::string shaderSource;
	std::vector<uint32_t> spirvData;

	// 1. Preprocess
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		// Include
		options.SetIncluder(std::make_unique<ShaderIncluder>());

		// Definition
		options.AddMacroDefinition(BackendToDefinition[(size_t)RenderCore::GetBackend()]);

		shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(
			info.m_rowData.c_str(), info.m_rowData.size(),
			shaderKind, name, options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			SL_LOG_ERROR("Shader preprocess failed: \"{}\"", name);
			SL_LOG_ERROR(result.GetErrorMessage());
			return "";
		}

		shaderSource = { result.cbegin(), result.cend() };
	}

	// 2. Compile to SPIR-V
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		// TODO: Shader stores uniform location
		// Shaderc without debug info will lose uniform name infomation,
		// which make us can not get uniform location by uniform name.
		options.SetGenerateDebugInfo();

#if defined(SL_DEBUG)
		options.SetOptimizationLevel(shaderc_optimization_level_zero);
#else
		options.SetOptimizationLevel(shaderc_optimization_level_performance);
#endif

		// TODO: Start from vulkan glsl in the future.
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

		shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
			shaderSource.c_str(), shaderSource.size(),
			shaderKind, name, "main", options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			SL_LOG_ERROR("Compile shader to SPIR-V failed: \"{}\"", name);
			SL_LOG_ERROR(result.GetErrorMessage());
			return "";
		}

		spirvData = { result.cbegin(), result.cend() };
	}

	// 3. Compile to shader source
	{
		spirv_cross::CompilerGLSL glsl(std::move(spirvData));
		
		// TODO: Reflection
		spirv_cross::ShaderResources resources = glsl.get_shader_resources();

		spirv_cross::CompilerGLSL::Options options;
		options.version = 450;
		glsl.set_common_options(options);

		std::string source = glsl.compile();

		SL_LOG_DEBUG(source);

		return source;

		// return glsl.compile();
	}

	return "";
}

} // namespace sl
