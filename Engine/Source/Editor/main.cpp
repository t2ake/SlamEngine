#include "Event/KeyEvent.h"
#include "Log/Log.h"

#include <iostream>

int main()
{
	sl::Log::Init();

	auto lambda = [](sl::Event& event)
	{
		SL_EDITOR_DEBUG(event.ToString());
		return true;
	};

	sl::KeyPressedEvent event{ 42 };
	sl::EventDispatcher dis{ event };
	dis.Dispatch<sl::KeyPressedEvent>(lambda);
	dis.Dispatch<sl::KeyReleasedEvent>(lambda);

	return 0;
}
