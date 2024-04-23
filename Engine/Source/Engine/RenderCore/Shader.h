#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include <string>

namespace sl
{

// TODO: Shader resource
class Shader
{
public:
	static Shader *Creat(std::string vsSrc, std::string fsSrc);

public:
	virtual ~Shader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	// TEMPORARY
	virtual void UploadUniformMat4(std::string name, glm::mat4 mat) const = 0;
};

} // namespace sl
