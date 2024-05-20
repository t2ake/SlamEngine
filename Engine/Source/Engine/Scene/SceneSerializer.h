#pragma once

#include <string>

namespace sl
{

class SceneSerializer
{
public:
	SceneSerializer() = delete;

	static void SerializeYAML(std::string_view sceneName = "MyScene");
	static void SerializeBinary();

	static bool DeserializeYAML(std::string_view sceneName = "MyScene");
	static bool DeserializeBinary();
};

} // namespace sl
