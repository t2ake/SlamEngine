#include "SceneSerializer.h"

#include "Core/Log.h"
#include "Core/Path.hpp"
#include "Resource/FileIO.hpp"
#include "Scene/ECSWorld.h"
#include "Scene/YAMLConvert.hpp"
#include "Utils/ProfilerCPU.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace sl
{

namespace
{

std::string GetSceneFilePath(std::string_view name)
{
	std::string relative = "Scene/";
	relative += name.data();
	relative += ".yaml";

	return Path::FromeAsset(relative);
}

} // namespace

void SceneSerializer::SerializeYAML(std::string_view sceneName)
{
	SL_PROFILE;

	std::string filePath = GetSceneFilePath(sceneName);
	SL_LOG_INFO("Serialize scene data file: \"{}\"", filePath);

	YAML::Emitter out;
	out << YAML::Comment("Slam Engine scene data file, generated by yaml-cpp.");

	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << sceneName.data();
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

	auto view = sl::ECSWorld::GetRegistry().view<sl::TagComponent>();
	for (auto entity : view)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity ID" << YAML::Value << (uint32_t)entity;

		if (auto *pTag = sl::ECSWorld::GetRegistry().try_get<sl::TagComponent>(entity); pTag)
		{
			out << YAML::Key << "Tag Component";
			out << YAML::BeginMap;
			out << YAML::Key << "Name" << YAML::Value << pTag->m_name;
			out << YAML::EndMap;
		}

		if (auto *pTransform = sl::ECSWorld::GetRegistry().try_get<sl::TransformComponent>(entity); pTransform)
		{
			out << YAML::Key << "Transform Component";
			out << YAML::BeginMap;
			out << YAML::Key << "Position" << YAML::Value << pTransform->m_position;
			out << YAML::Key << "Rotation" << YAML::Value << pTransform->m_rotation;
			out << YAML::Key << "Scale" << YAML::Value << pTransform->m_scale;
			out << YAML::EndMap;
		}

		if (auto *pCamera = sl::ECSWorld::GetRegistry().try_get<sl::CameraComponent>(entity); pCamera)
		{
			out << YAML::Key << "Camera Component";
			out << YAML::BeginMap;
			
			out << YAML::Key << "Main Camera" << YAML::Value << pCamera->m_isMainCamera;

			out << YAML::Key << "Projection Type" << YAML::Value << pCamera->m_projectionType;

			out << YAML::Key << "Perspective";
			out << YAML::BeginMap;
			out << YAML::Key << "FOV" << YAML::Value << pCamera->m_fov;
			out << YAML::Key << "Near Plane" << YAML::Value << pCamera->m_nearPlane;
			out << YAML::Key << "Far Plane" << YAML::Value << pCamera->m_farPlane;
			out << YAML::EndMap;

			out << YAML::Key << "Orthographic";
			out << YAML::BeginMap;
			out << YAML::Key << "Size" << YAML::Value << pCamera->m_orthoSize;
			out << YAML::Key << "Near Clip" << YAML::Value << pCamera->m_orthoNearClip;
			out << YAML::Key << "Far Clip" << YAML::Value << pCamera->m_orthoFarClip;
			out << YAML::EndMap;

			out << YAML::Key << "Controller";
			out << YAML::BeginMap;
			out << YAML::Key << "Rotate Speed" << YAML::Value << pCamera->m_rotateSpeed;
			out << YAML::Key << "Move Speed" << YAML::Value << pCamera->m_maxMoveSpeed;
			out << YAML::Key << "Acceleration" << YAML::Value << pCamera->m_maxSpeedToAcceleration;
			out << YAML::Key << "Shift Multiplier" << YAML::Value << pCamera->m_moveSpeedKeyShiftMultiplier;
			out << YAML::Key << "Scroll Multiplier" << YAML::Value << pCamera->m_moveSpeedMouseScrollMultiplier;
			out << YAML::EndMap;

			out << YAML::EndMap; // CameraComponent
		}

		if (auto *pRendering = sl::ECSWorld::GetRegistry().try_get<sl::RenderingComponent>(entity); pRendering)
		{
			out << YAML::Key << "Rendering Component";
			out << YAML::BeginMap;

			out << YAML::Key << "Mesh" << YAML::Value << (pRendering->m_optMeshResourceName ? pRendering->m_optMeshResourceName->c_str() : "");
			out << YAML::Key << "Material" << YAML::Value << (pRendering->m_optMaterialResourceName ? pRendering->m_optMaterialResourceName->c_str() : "");

			out << YAML::Key << "Base Shader" << YAML::Value << (pRendering->m_optBaseShaderResourceName ? pRendering->m_optBaseShaderResourceName->c_str() : "");
			out << YAML::Key << "ID Shader" << YAML::Value << (pRendering->m_optIDShaderResourceName ? pRendering->m_optIDShaderResourceName->c_str() : "");

			out << YAML::EndMap;
		}

		if (auto *pCornerstone = sl::ECSWorld::GetRegistry().try_get<sl::CornerstoneComponent>(entity); pCornerstone)
		{
			out << YAML::Key << "Cornerstone Component";
			out << YAML::BeginMap;

			out << YAML::Key << "Info" << YAML::Value << pCornerstone->m_info;

			out << YAML::EndMap;
		}

		out << YAML::EndMap; // Entity
	}
	out << YAML::EndSeq; // Entities
	out << YAML::EndMap; // Scene

	std::ofstream fileStream{ filePath };
	if (fileStream.fail())
	{
		SL_LOG_ERROR("Failed to serialize scene!");
		return;
	}

	fileStream << out.c_str();
}

void SceneSerializer::SerializeBinary(std::string_view sceneName)
{
	SL_PROFILE;
}

bool SceneSerializer::DeserializeYAML(std::string_view sceneName)
{
	SL_PROFILE;

	std::string filePath = GetSceneFilePath(sceneName);
	SL_LOG_INFO("Deserialize scene data file: \"{}\"", filePath);

	YAML::Node data = YAML::Load(FileIO::ReadString(filePath));
	if (!data["Scene"])
	{
		SL_LOG_ERROR("Unknown scene data file!");
		return false;
	}

	SL_LOG_ERROR("Just some test code here, will not actually open a new scene.");
	std::string scene = data["Scene"].as<std::string>();
	SL_LOG_TRACE("Scene: {}", scene);

	if (auto entities = data["Entities"]; entities)
	{
		for (const auto entity : entities)
		{
			// Entity ID
			if (auto entityID = entity["Entity ID"]; entityID)
			{
				SL_LOG_TRACE("\tEntity ID: {}", entityID.as<uint32_t>());
			}

			// Tag Component
			if (auto tag = entity["Tag Component"]; tag)
			{
				SL_LOG_TRACE("\t\tTag Component:");
				if (auto name = tag["Name"]; name)
				{
					SL_LOG_TRACE("\t\t\tName: {}", name.as<std::string>());
				}
			}

			// Transform Component
			if (auto transform = entity["Transform Component"]; transform)
			{
				SL_LOG_TRACE("\t\tTransform Component:");
				if (auto position = transform["Position"]; position)
				{
					SL_LOG_TRACE("\t\t\tPosition: {}", position.as<glm::vec3>());
				}
				if (auto rotation = transform["Rotation"]; rotation)
				{
					SL_LOG_TRACE("\t\t\tRotation: {}", rotation.as<glm::vec3>());
				}
				if (auto scale = transform["Scale"]; scale)
				{
					SL_LOG_TRACE("\t\t\tScale: {}", scale.as<glm::vec3>());
				}
			}

			// Camera Component
			if (auto camera = entity["Camera Component"]; camera)
			{
				SL_LOG_TRACE("\t\tCamera Component:");

				if (auto mainCamera = camera["Main Camera"]; mainCamera)
				{
					SL_LOG_TRACE("\t\t\tMain Camera: {}", mainCamera.as<bool>());
				}

				if (auto projectionType = camera["Projection Type"]; projectionType)
				{
					SL_LOG_TRACE("\t\t\tProjection Type: {}", nameof::nameof_enum(projectionType.as<sl::ProjectionType>()));
				}

				if (auto perspective = camera["Perspective"]; perspective)
				{
					SL_LOG_TRACE("\t\t\tPerspective:");
					if (auto fov = perspective["FOV"]; fov)
					{
						SL_LOG_TRACE("\t\t\t\tFOV: {}", fov.as<float>());
					}
					if (auto nearPlan = perspective["Near Plane"]; nearPlan)
					{
						SL_LOG_TRACE("\t\t\t\tNear Plane: {}", nearPlan.as<float>());
					}
					if (auto farPaln = perspective["Far Plane"]; farPaln)
					{
						SL_LOG_TRACE("\t\t\t\tFar Plane: {}", farPaln.as<float>());
					}
				}
				if (auto orthographic = camera["Orthographic"]; orthographic)
				{
					SL_LOG_TRACE("\t\t\tOrthographic:");
					if (auto size = orthographic["Size"]; size)
					{
						SL_LOG_TRACE("\t\t\t\tSize: {}", size.as<float>());
					}
					if (auto nearClip = orthographic["Near Clip"]; nearClip)
					{
						SL_LOG_TRACE("\t\t\t\tNear Clip: {}", nearClip.as<float>());
					}
					if (auto farClip = orthographic["Far Clip"]; farClip)
					{
						SL_LOG_TRACE("\t\t\t\tFar Clip: {}", farClip.as<float>());
					}
				}
				if (auto controller = camera["Controller"]; controller)
				{
					SL_LOG_TRACE("\t\t\tController:");
					if (auto rotateSpeed = controller["Rotate Speed"]; rotateSpeed)
					{
						SL_LOG_TRACE("\t\t\t\tRotate Speed: {}", rotateSpeed.as<float>());
					}
					if (auto moveSpeed = controller["Move Speed"]; moveSpeed)
					{
						SL_LOG_TRACE("\t\t\t\tMove Speed: {}", moveSpeed.as<float>());
					}
					if (auto acceleration = controller["Acceleration"]; acceleration)
					{
						SL_LOG_TRACE("\t\t\t\tAcceleration: {}", acceleration.as<float>());
					}
					if (auto shiftMultiplier = controller["Shift Multiplier"]; shiftMultiplier)
					{
						SL_LOG_TRACE("\t\t\t\tShift Multiplier: {}", shiftMultiplier.as<float>());
					}
					if (auto scrollMultiplier = controller["Scroll Multiplier"]; scrollMultiplier)
					{
						SL_LOG_TRACE("\t\t\t\tScroll Multiplier: {}", scrollMultiplier.as<float>());
					}
				}
			}

			// Redering Component
			if (auto redering = entity["Rendering Component"]; redering)
			{
				SL_LOG_TRACE("\t\tRendering Component:");
				if (auto mesh = redering["Mesh"]; mesh)
				{
					SL_LOG_TRACE("\t\t\tMesh: {}", mesh.as<std::string>());
				}
				if (auto material = redering["Material"]; material)
				{
					SL_LOG_TRACE("\t\t\tMaterial: {}", material.as<std::string>());
				}
				if (auto baseShader = redering["Base Shader"]; baseShader)
				{
					SL_LOG_TRACE("\t\t\tBase Shader: {}", baseShader.as<std::string>());
				}
				if (auto IDShader = redering["ID Shader"]; IDShader)
				{
					SL_LOG_TRACE("\t\t\tID Shader: {}", IDShader.as<std::string>());
				}
			}

			// Cornerstone Component
			if (auto cornerstone = entity["Cornerstone Component"]; cornerstone)
			{
				SL_LOG_TRACE("\t\tCornerstone Component:");
				if (auto info = cornerstone["Info"]; info)
				{
					SL_LOG_TRACE("\t\t\tInfo: {}", info.as<std::string>());
				}
			}
		}
	}

	return true;
}

bool SceneSerializer::DeserializeBinary(std::string_view sceneName)
{
	SL_PROFILE;

	return false;
}

} // namespace sl
