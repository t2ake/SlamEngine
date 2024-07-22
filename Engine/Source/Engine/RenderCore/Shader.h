#pragma once

#include "Core/Defines.h"

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <string>

namespace sl
{

class Shader
{
public:
	static Shader *Create(std::string_view vsSource, std::string_view fsSource);
	static Shader *Create(std::string_view shaderSource, ShaderType type);

public:
	virtual ~Shader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void UploadUniform(int location, int value) = 0;
	virtual void UploadUniform(int location, const glm::ivec2 &value) = 0;
	virtual void UploadUniform(int location, const glm::ivec3 &value) = 0;
	virtual void UploadUniform(int location, const glm::ivec4 &value) = 0;

	virtual void UploadUniform(int location, unsigned int value) = 0;
	virtual void UploadUniform(int location, const glm::uvec2 &value) = 0;
	virtual void UploadUniform(int location, const glm::uvec3 &value) = 0;
	virtual void UploadUniform(int location, const glm::uvec4 &value) = 0;

	virtual void UploadUniform(int location, float value) = 0;
	virtual void UploadUniform(int location, const glm::vec2 &value) = 0;
	virtual void UploadUniform(int location, const glm::vec3 &value) = 0;
	virtual void UploadUniform(int location, const glm::vec4 &value) = 0;

	virtual void UploadUniform(int location, const glm::mat2 &value) = 0;
	virtual void UploadUniform(int location, const glm::mat3 &value) = 0;
	virtual void UploadUniform(int location, const glm::mat4 &value) = 0;
};

} // namespace sl
