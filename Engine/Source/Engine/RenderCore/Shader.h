#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <string>

namespace sl
{

enum class ShaderType
{
	VertexShader,
	FragmentShader,
	ComputeShader,
	// Geometry
	// Raytracing
};

enum class ShaderProgramType
{
	Standard,
	Compute,
	// Vertex only
};

// TODO: Shader resource
// TODO: Shader should hold a mapping from Texture to slot.
class Shader
{
public:
	static Shader *Creat(std::string name, std::string vsSrc, std::string fsSrc);

public:
	virtual ~Shader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void UploadUniform(const std::string &name, int value) = 0;
	virtual void UploadUniform(const std::string &name, const glm::ivec2 &value) = 0;
	virtual void UploadUniform(const std::string &name, const glm::ivec3 &value) = 0;
	virtual void UploadUniform(const std::string &name, const glm::ivec4 &value) = 0;

	virtual void UploadUniform(const std::string &name, unsigned int value) = 0;
	virtual void UploadUniform(const std::string &name, const glm::uvec2 &value) = 0;
	virtual void UploadUniform(const std::string &name, const glm::uvec3 &value) = 0;
	virtual void UploadUniform(const std::string &name, const glm::uvec4 &value) = 0;

	virtual void UploadUniform(const std::string &name, float value) = 0;
	virtual void UploadUniform(const std::string &name, const glm::vec2 &value) = 0;
	virtual void UploadUniform(const std::string &name, const glm::vec3 &value) = 0;
	virtual void UploadUniform(const std::string &name, const glm::vec4 &value) = 0;

	virtual void UploadUniform(const std::string &name, const glm::mat2 &value) = 0;
	virtual void UploadUniform(const std::string &name, const glm::mat3 &value) = 0;
	virtual void UploadUniform(const std::string &name, const glm::mat4 &value) = 0;
};

} // namespace sl
