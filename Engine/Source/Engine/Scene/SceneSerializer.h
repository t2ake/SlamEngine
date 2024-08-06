#pragma once

#include <string>

namespace sl
{

class SceneSerializer final
{
public:
	SceneSerializer() = delete;
	SceneSerializer(const SceneSerializer &) = delete;
	SceneSerializer &operator=(const SceneSerializer &) = delete;
	SceneSerializer(SceneSerializer &&) = delete;
	SceneSerializer &operator=(SceneSerializer &&) = delete;
	~SceneSerializer() = delete;

	static void SerializeYAML(std::string_view sceneName);
	static void SerializeBinary();

	static bool DeserializeYAML(std::string_view sceneName);
	static bool DeserializeBinary();
};

} // namespace sl
