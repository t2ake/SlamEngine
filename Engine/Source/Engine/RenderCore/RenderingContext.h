#pragma once

struct GLFWwindow;

namespace sl
{

class RenderingContext
{
public:
	virtual void Init(GLFWwindow *pWindow) = 0;
	virtual void SwapBuffers() = 0;
	virtual void Shutdown() = 0;
};

} // namespace sl
