#pragma once

#include "Core/Defines.h"
#include "Resource/Resource.h"

#include <memory>
#include <string>
#include <vector>

namespace sl
{

class Shader;

struct ShaderInfo
{
	ShaderType m_type;
	std::string m_name;
	std::string m_assetPath;
	std::string m_rowData;
	std::vector<uint32_t> m_spirvData;
};

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
	virtual void DestroyRawData() override;

	ShaderProgramType m_programType;
	ShaderInfo m_shaders[2];

	std::unique_ptr<Shader> m_pShaderProgram;
};

} // namespace sl
