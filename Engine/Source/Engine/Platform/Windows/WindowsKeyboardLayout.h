#pragma once

#if defined(SL_WINDOWS)

#include <Windows.h>

namespace sl
{

class WindowsKeyboardLayout final
{
public:
	static void SwitchToEnglish()
	{
		if (PRIMARYLANGID(LOWORD(GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL)))) != LANG_ENGLISH)
		{
			HKL hklEnglish = LoadKeyboardLayout(L"00000409", KLF_ACTIVATE);
			ActivateKeyboardLayout(hklEnglish, KLF_SETFORPROCESS);
		}
	}
};

} // namespace sl

#endif
