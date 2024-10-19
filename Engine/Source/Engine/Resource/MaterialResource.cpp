#include "MaterialResource.h"

#include "Utils/ProfilerCPU.h"

namespace sl
{

MaterialResource::MaterialResource(std::string_view path) :
	m_assetPath(path)
{
	SetState(ResourceState::Importing);
}

MaterialResource::~MaterialResource()
{
	OnDestroy();
}

void MaterialResource::OnImport()
{
	SL_PROFILE;

	SetState(ResourceState::Building);
}

void MaterialResource::OnBuild()
{
	SL_PROFILE;

	SetState(ResourceState::Uploading);
}

void MaterialResource::OnLoad()
{
	SL_PROFILE;

	SetState(ResourceState::Uploading);
}

void MaterialResource::OnUpload()
{
	SL_PROFILE;

	SetState(ResourceState::Ready);
}

void MaterialResource::OnReady()
{
	static uint8_t frameCount = 0;
	if (frameCount <= 60 && frameCount++ == 60)
	{
		DestroyCPUData();
	}
}

void MaterialResource::OnDestroy()
{
	SL_PROFILE;

	DestroyCPUData();

	SetState(ResourceState::Destroyed);
}

void MaterialResource::DestroyCPUData()
{
	SL_PROFILE;
}

} // namespace sl
