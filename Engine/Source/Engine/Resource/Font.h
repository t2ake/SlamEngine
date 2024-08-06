#pragma once

#include <imgui/imgui.h>

namespace sl
{

class Font final
{
public:
	Font() = delete;
	Font(const Font &) = delete;
	Font &operator=(const Font &) = delete;
	Font(Font &&) = delete;
	Font &operator=(Font &&) = delete;
	~Font() = delete;

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
