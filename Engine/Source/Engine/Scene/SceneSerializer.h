#pragma once

#include <string>

namespace sl
{

class SceneSerializer
{
public:
	SceneSerializer() = delete;

	static void SerializeYAML(std::string_view sceneName);
	static void SerializeBinary();

	static bool DeserializeYAML(std::string_view sceneName);
	static bool DeserializeBinary();
};

} // namespace sl
