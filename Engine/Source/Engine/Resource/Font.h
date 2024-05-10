#pragma once

#include <imgui/imgui.h>

namespace sl
{

class Font final
{
public:
	static void SetRegular(ImFont *pFont) { pRegular = pFont; }
	static ImFont *GetRegular() { return pRegular; }

	static void SetBold(ImFont *pFont) { pBold = pFont; }
	static ImFont *GetBold() { return pBold; }

	static void SetThin(ImFont *pFont) { pThin = pFont; }
	static ImFont *GetThin() { return pThin; }

private:
	inline static ImFont *pRegular = nullptr;
	inline static ImFont *pBold = nullptr;
	inline static ImFont *pThin = nullptr;
};

} // namespace sl
