#pragma once

#include <cstdint>

namespace sl
{

enum class ResourceStatus : uint8_t
{
	Importing = 0,
	Building,
	Loading,
	Uploading,
	Ready,
	Destroying,
	Destroyed,
};

enum class ResourcesType : uint8_t
{
	Mesh = 0,
	Bone,
	Animation,
	Texture,
	Shader,
	Material,
};

class Resource
{
public:
	virtual	~Resource() = default;

	void SetStatus(ResourceStatus status) { m_status = status; }
	ResourceStatus GetStatus() { return m_status; }
	bool IsReady() { return m_status == ResourceStatus::Ready; }

	void Update();

	// Import asset file in original format.
	virtual void OnImport() = 0;
	// Compile to internal format.
	virtual void OnBuild() = 0;
	// Read serialized internal format file.
	virtual void OnLoad() = 0;
	// Creat GPU handle.
	virtual void OnUpload() = 0;
	// Destroy CPU data deferred.
	virtual void OnReady() = 0;
	// Destroy GPU data
	virtual void OnDestroy() = 0;

private:
	virtual void DestroyCPUData() = 0;

	ResourceStatus m_status;
};

} // namespace sl
