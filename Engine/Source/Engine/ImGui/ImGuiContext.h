#pragma once

namespace sl
{

class ImGuiContext final
{
public:
	ImGuiContext() = delete;

	static void Init();
	static void Shutdown();

	static void BeginFrame();
	static void EndFrame();
};

} // namespace sl
