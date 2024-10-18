#include "MaterialResource.h"

#include "Utils/ProfilerCPU.h"

namespace sl
{

MaterialResource::MaterialResource(std::string_view path) :
	m_assetPath(path)
{
	SetStatus(ResourceStatus::Importing);
}

MaterialResource::~MaterialResource()
{
	OnDestroy();
}

void MaterialResource::OnImport()
{
	SL_PROFILE;

	SetStatus(ResourceStatus::Building);
}

void MaterialResource::OnBuild()
{
	SL_PROFILE;

	SetStatus(ResourceStatus::Uploading);
}

void MaterialResource::OnLoad()
{
	SL_PROFILE;

	SetStatus(ResourceStatus::Uploading);
}

void MaterialResource::OnUpload()
{
	SL_PROFILE;

	SetStatus(ResourceStatus::Ready);
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

	SetStatus(ResourceStatus::Destroyed);
}

void MaterialResource::DestroyCPUData()
{
	SL_PROFILE;
}

} // namespace sl
