#include "MeshResource.h"

#include "Core/Log.h"
#include "RenderCore/VertexArray.h"
#include "Utils/ProfilerCPU.h"

namespace sl
{

MeshResource::~MeshResource()
{
	OnDestroy();
}

void MeshResource::OnImport()
{
	SL_PROFILE;

	m_state = ResourceState::Building;
}

void MeshResource::OnBuild()
{
	SL_PROFILE;

	m_state = ResourceState::Uploading;
}

void MeshResource::OnLoad()
{
	SL_PROFILE;

	m_state = ResourceState::Uploading;
}

void MeshResource::OnUpload()
{
	SL_PROFILE;

	sl::VertexBuffer *pVertexBuffer = sl::VertexBuffer::Create(m_verticesRowData.data(), m_verticesRowData.size() * sizeof(float));
	pVertexBuffer->SetLayout(m_layout);
	sl::IndexBuffer *pIndexBuffer = sl::IndexBuffer::Create(m_indicesRowData.data(), m_indicesRowData.size() * sizeof(uint32_t));

	m_pVertexArray.reset(sl::VertexArray::Create());
	m_pVertexArray->SetVertexBuffer(pVertexBuffer);
	m_pVertexArray->SetIndexBuffer(pIndexBuffer);

	m_state = ResourceState::Ready;
}

void MeshResource::OnReady()
{
	if (m_destroyDelay <= 60 && m_destroyDelay++ == 60)
	{
		DestroyCPUData();
	}
}

void MeshResource::OnDestroy()
{
	SL_PROFILE;

	DestroyCPUData();
	m_pVertexArray.reset();

	m_state = ResourceState::Destroyed;
}

void MeshResource::DestroyCPUData()
{
	SL_PROFILE;

	m_verticesRowData.clear();
	std::vector<float>().swap(m_verticesRowData);
	m_indicesRowData.clear();
	std::vector<uint32_t>().swap(m_indicesRowData);
}

} // namespace sl
