#pragma once

namespace sl
{

class KeyboardLayout final
{
public:
	KeyboardLayout() = delete;
	KeyboardLayout(const KeyboardLayout &) = delete;
	KeyboardLayout &operator=(const KeyboardLayout &) = delete;
	KeyboardLayout(KeyboardLayout &&) = delete;
	KeyboardLayout &operator=(KeyboardLayout &&) = delete;
	~KeyboardLayout() = delete;

	static void SwitchToEnglish();
};

} // namespace sl
