#pragma once

struct GLFWwindow;

namespace sl
{

class RenderContext
{
public:
	static RenderContext *Create(GLFWwindow *pWindow);

public:
	virtual ~RenderContext() = default;

	virtual void Bind() = 0;
	virtual void SwapBuffers() = 0;
};

} // namespace sl
