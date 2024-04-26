#pragma once

#include "Camera/Camera.h"
#include "Layer/Layer.h"

namespace sl
{

class VertexArray;
class Texture2D;
class Shader;

}

class SandboxLayer : public sl::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer() override;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(sl::Event &event) override;

	virtual void BeginFrame() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void EndFrame() override;
private:
	sl::Camera m_camera;

	sl::VertexArray *m_pVertexArray;
	sl::Texture2D *m_pTextureJoucho;
	sl::Texture2D *m_pTextureLogo;
	sl::Shader *m_pShader;
};
