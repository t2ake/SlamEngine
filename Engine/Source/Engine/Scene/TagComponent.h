#pragma once

#include <string>

namespace sl
{

struct TagComponent
{
	void Reset()
	{
		m_name = "Default Name";
	}

	std::string m_name;
};

} // namespace sl
