#pragma once

namespace sl
{

class SceneSerializer
{
public:
	SceneSerializer() = delete;

	static void SerializeYAML(const char *sceneName = "MyScene");
	static void SerializeBinary();

	static void DeserializeYAML();
	static void DeserializeBinary();
};

} // namespace sl
