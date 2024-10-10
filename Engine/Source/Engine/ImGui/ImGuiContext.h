#pragma once

#include <string>

struct ImFont;

namespace sl
{

class ImGuiContext final
{
public:
	ImGuiContext() = delete;

	static void Init(void *pNativeWindow, void *pRenderContext);
	static void Shutdown();

	static void NewFrame();
	static void Submit();
	static void OnEvent(void *pSDLEvent);

	static void SetUsingMouse(bool enable);

	static ImFont *GetRegularFont() { return m_pRegularFont; }
	static ImFont *GetBoldFont() { return m_pBoldFont; }
	static ImFont *GetThinFont() { return m_pThinFont; }

	static void SetMouseVisibility(bool vis) { m_showMouse = vis; }

private:
	static void SetColor();
	static void SetStyle();

	// ImGui holds the ownership of these datas.
	inline static ImFont *m_pRegularFont;
	inline static ImFont *m_pBoldFont;
	inline static ImFont *m_pThinFont;

	inline static bool m_showMouse = true;
};

} // namespace sl
