#pragma once

#include "RenderCore/Layout.h"
#include "Resource/Resource.h"

#include <memory>

#include <glm/vec3.hpp>

namespace sl
{

class VertexArray;

class MeshResource : public Resource
{
public:
	MeshResource(std::string_view path);
	virtual ~MeshResource() override;

	virtual void OnImport() override;
	virtual void OnBuild() override;
	virtual void OnLoad() override;
	virtual void OnUpload() override;
	virtual void OnReady() override;
	virtual void OnDestroy() override;
	virtual void DestroyCPUData() override;

	VertexArray *GetVertexArray() { return m_pVertexArray.get(); }

	std::string m_assetPath;

	VertexLayout m_layout;
	std::vector<float> m_verticesRowData;
	std::vector<uint32_t> m_indicesRowData;

	std::unique_ptr<VertexArray> m_pVertexArray;
};

} // namespace sls
