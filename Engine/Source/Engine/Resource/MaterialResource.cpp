#include "MaterialResource.h"

#include "Utils/ProfilerCPU.h"

namespace sl
{

MaterialResource::~MaterialResource()
{
	OnDestroy();
}

void MaterialResource::OnImport()
{
	SL_PROFILE;

	m_state = ResourceState::Building;
}

void MaterialResource::OnBuild()
{
	SL_PROFILE;

	m_state = ResourceState::Uploading;
}

void MaterialResource::OnLoad()
{
	SL_PROFILE;

	m_state = ResourceState::Uploading;
}

void MaterialResource::OnUpload()
{
	SL_PROFILE;

	m_state = ResourceState::Ready;
}

void MaterialResource::OnReady()
{
	if (m_destroyDelay <= 60 && m_destroyDelay++ == 60)
	{
		DestroyCPUData();
	}
}

void MaterialResource::OnDestroy()
{
	SL_PROFILE;

	DestroyCPUData();

	m_state = ResourceState::Destroyed;
}

void MaterialResource::DestroyCPUData()
{
	SL_PROFILE;
}

} // namespace sl
