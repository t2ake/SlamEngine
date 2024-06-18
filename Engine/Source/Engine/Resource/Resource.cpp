#include "Resource.h"

#include "Core/Log.h"

namespace sl
{

void Resource::Update()
{
	switch (m_status)
	{
		case ResourceStatus::Importing:
		{
			OnImport();
			break;
		}
		case ResourceStatus::Building:
		{
			OnBuild();
			break;
		}
		case ResourceStatus::Loading:
		{
			OnLoad();
			break;
		}
		case ResourceStatus::Uploading:
		{
			OnUpload();
			break;
		}
		case ResourceStatus::Ready:
		{
			OnReady();
			break;
		}
		case ResourceStatus::Destroying:
		{
			OnDestroy();
			break;
		}
		case ResourceStatus::Destroyed:
		{
			break;
		}
		default:
		{
			SL_ASSERT_INFO(false, "Unknown resource state!");
			break;
		}
	}
}

} // namespace sl
