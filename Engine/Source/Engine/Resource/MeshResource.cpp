#include "MeshResource.h"

#include "RenderCore/VertexArray.h"

namespace sl
{

MeshResource::MeshResource(std::string_view path) :
	m_assetPath(path)
{
	SetStatus(ResourceStatus::Importing);
}

MeshResource::~MeshResource()
{
	OnDestroy();
}

void MeshResource::OnImport()
{
	// TODO: Import model file.
	SetStatus(ResourceStatus::Building);
}

void MeshResource::OnBuild()
{
	SetStatus(ResourceStatus::Uploading);
}

void MeshResource::OnLoad()
{
	SetStatus(ResourceStatus::Uploading);
}

void MeshResource::OnUpload()
{
	sl::VertexBuffer *pVertexBuffer = sl::VertexBuffer::Create(m_verticesRowData.data(), m_verticesRowData.size() * sizeof(float));
	pVertexBuffer->SetLayout(m_layout);
	sl::IndexBuffer *pIndexBuffer = sl::IndexBuffer::Create(m_indicesowData.data(), m_indicesowData.size() * sizeof(uint32_t));

	m_pVertexArray.reset(sl::VertexArray::Create());
	m_pVertexArray->SetVertexBuffer(pVertexBuffer);
	m_pVertexArray->SetIndexBuffer(pIndexBuffer);

	SetStatus(ResourceStatus::Ready);
}

void MeshResource::OnReady()
{
	static uint32_t frameCount = 0;
	if (frameCount < 60)
	{
		++frameCount;
	}
	else if (frameCount == 60)
	{
		DestroyRawData();
	}
}

void MeshResource::OnDestroy()
{
	DestroyRawData();
	m_pVertexArray.reset();

	SetStatus(ResourceStatus::Destroyed);
}

void MeshResource::DestroyRawData()
{
	m_verticesRowData.clear();
	std::vector<float>().swap(m_verticesRowData);
	m_indicesowData.clear();
	std::vector<uint32_t>().swap(m_indicesowData);
}

} // namespace sl
