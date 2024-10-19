#pragma once

#include <cstdint>

namespace sl
{

enum class ResourceState : uint8_t
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

	void SetState(ResourceState state) { m_state = state; }
	ResourceState GetState() const { return m_state; }
	bool IsReady() { return m_state == ResourceState::Ready; }

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
	// Destroy both CPU and GPU data.
	virtual void OnDestroy() = 0;
	virtual void DestroyCPUData() = 0;

	ResourceState m_state;
};

} // namespace sl
