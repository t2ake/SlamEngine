#include "SceneSerializer.h"

#include "Core/Log.h"
#include "Core/Path.hpp"
#include "Scene/ECSWorld.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace sl
{

YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec2 &v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec3 &v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec4 &v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

void SceneSerializer::SerializeYAML(const char *sceneName)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << sceneName;
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

	auto view = sl::ECSWorld::GetRegistry().view<sl::TagComponent>();
	for (auto entity : view)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;

		if (auto *pTag = sl::ECSWorld::GetRegistry().try_get<sl::TagComponent>(entity); pTag)
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Name" << YAML::Value << pTag->m_name;
			out << YAML::EndMap;
		}

		if (auto *pTransform = sl::ECSWorld::GetRegistry().try_get<sl::TransformComponent>(entity); pTransform)
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Position" << YAML::Value << pTransform->m_position;
			out << YAML::Key << "Rotation" << YAML::Value << pTransform->m_rotation;
			out << YAML::Key << "Scale" << YAML::Value << pTransform->m_scale;
			out << YAML::EndMap;
		}

		if (auto *pCamera = sl::ECSWorld::GetRegistry().try_get<sl::CameraComponent>(entity); pCamera)
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;
			
			out << YAML::Key << "Perspective";
			out << YAML::BeginMap;
			out << YAML::Key << "FOV" << YAML::Value << pCamera->m_fov;
			out << YAML::Key << "Near Plane" << YAML::Value << pCamera->m_nearPlane;
			out << YAML::Key << "Far Plane" << YAML::Value << pCamera->m_farPlane;
			out << YAML::EndMap; // Perspective

			out << YAML::Key << "Orthographic";
			out << YAML::BeginMap;
			out << YAML::Key << "Size" << YAML::Value << pCamera->m_orthoSize;
			out << YAML::Key << "Near Clip" << YAML::Value << pCamera->m_orthoNearClip;
			out << YAML::Key << "Far Clip" << YAML::Value << pCamera->m_orthoFarClip;
			out << YAML::EndMap; // Orthographic

			out << YAML::Key << "Controller";
			out << YAML::BeginMap;
			out << YAML::Key << "Rotate Speed" << YAML::Value << pCamera->m_rotateSpeed;
			out << YAML::Key << "Move Speed" << YAML::Value << pCamera->m_maxMoveSpeed;
			out << YAML::Key << "Acceleration" << YAML::Value << pCamera->m_maxSpeedToAcceleration;
			out << YAML::Key << "Shift Multiplier" << YAML::Value << pCamera->m_moveSpeedKeyShiftMultiplier;
			out << YAML::Key << "Scroll Multiplier" << YAML::Value << pCamera->m_moveSpeedMouseScrollMultiplier;
			out << YAML::EndMap; // Controller

			out << YAML::EndMap; // CameraComponent
		}

		if (auto *pRendering = sl::ECSWorld::GetRegistry().try_get<sl::RenderingComponent>(entity); pRendering)
		{
			out << YAML::Key << "RenderingComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Shader" << YAML::Value << "Shader name goes hera";

			out << YAML::EndMap;
		}

		if (auto *pCornerstone = sl::ECSWorld::GetRegistry().try_get<sl::CornerstoneComponent>(entity); pCornerstone)
		{
			out << YAML::Key << "CornerstoneComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Info" << YAML::Value << pCornerstone->m_info;

			out << YAML::EndMap;
		}

		out << YAML::EndMap; // Entity
	}
	out << YAML::EndSeq; // Entities

	out << YAML::EndMap;
	
	std::string outputPath = Path::FromeRoot(Path::Join("Project", sceneName));
	outputPath += ".yaml";
	
	std::ofstream fout{ outputPath };
	fout << out.c_str();
	
	SL_ENGINE_INFO("Scene data serialized to \"{}\"", outputPath);
}

void SceneSerializer::SerializeBinary()
{

}

void SceneSerializer::DeserializeYAML()
{

}

void SceneSerializer::DeserializeBinary()
{

}

} // namespace sl
