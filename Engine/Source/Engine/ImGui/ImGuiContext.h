#pragma once

#include <string>

namespace sl
{

class ImGuiContext final
{
public:
	ImGuiContext() = delete;

	static void Init(void *pNativeWindow);
	static void Shutdown();

	static void NewFrame();
	static void Submit();


private:
	static void SetColor();
	static void SetStyle();

	inline static std::string m_iniFilePath;
};

} // namespace sl
