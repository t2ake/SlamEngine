#pragma once

#include <string>

struct ImFont;

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

	static ImFont *GetRegular() { return m_pRegularFont; }
	static ImFont *GetBold() { return m_pBoldFont; }
	static ImFont *GetThin() { return m_pThinFont; }

private:
	static void SetColor();
	static void SetStyle();

	// ImGui holds the ownership of these datas.
	inline static ImFont *m_pRegularFont;
	inline static ImFont *m_pBoldFont;
	inline static ImFont *m_pThinFont;
};

} // namespace sl
