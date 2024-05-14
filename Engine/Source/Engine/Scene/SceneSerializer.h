#pragma once

namespace sl
{

class SceneSerializer
{
public:
	SceneSerializer() = delete;

	static void SerializeYAML(const char *sceneName = "MyScene");
	static void SerializeBinary();

	static bool DeserializeYAML(const char *sceneName = "MyScene");
	static bool DeserializeBinary();
};

} // namespace sl
