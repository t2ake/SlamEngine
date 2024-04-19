#pragma once

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
};

} // namespace sl
