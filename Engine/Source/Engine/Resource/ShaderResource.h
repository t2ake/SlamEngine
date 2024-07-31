#pragma once

#include "Core/Defines.h"
#include "Resource/Resource.h"
#include "Resource/ShaderInfo.h"

#include <memory>

namespace sl
{

class Shader;

class ShaderResource : public Resource
{
public:
	ShaderResource(std::string_view vsPath, std::string_view fsPath);
	ShaderResource(std::string_view path, ShaderProgramType type);
	virtual ~ShaderResource() override;

	virtual void OnImport() override;
	virtual void OnBuild() override;
	virtual void OnLoad() override;
	virtual void OnUpload() override;
	virtual void OnReady() override;
	virtual void OnDestroy() override;

	Shader* GetShaderProgram() { return m_pShaderProgram.get(); }

private:
	virtual void DestroyCPUData() override;

	ShaderProgramType m_programType;
	// TODO: Using template class to avoid ShaderInfo storage redundancy in non-standard shader situations.
	ShaderInfo m_shaders[2];

	std::unique_ptr<Shader> m_pShaderProgram;
};

} // namespace sl
