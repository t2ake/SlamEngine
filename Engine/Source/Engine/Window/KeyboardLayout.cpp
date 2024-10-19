#include "KeyboardLayout.h"

#include "Platform/Windows/WindowsKeyboardLayout.h"

namespace sl
{

void KeyboardLayout::SwitchToEnglish()
{
#if defined(SL_WINDOWS)
	WindowsKeyboardLayout::SwitchToEnglish();
#endif
}

} // namespace sl
