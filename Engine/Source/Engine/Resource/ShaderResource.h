#pragma once

#include "Core/Defines.h"
#include "Resource/Resource.h"

#include <memory>
#include <string>
#include <vector>

namespace sl
{

class Shader;

class ShaderResource : public Resource
{
public:
	struct ShaderInfo
	{
		ShaderType m_type;
		std::string m_assetPath;
		std::vector<std::byte> m_rowData;
	};

public:
	ShaderResource(std::string_view vsPath, std::string_view fsPath);
	ShaderResource(ShaderProgramType type, std::string_view path);
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
