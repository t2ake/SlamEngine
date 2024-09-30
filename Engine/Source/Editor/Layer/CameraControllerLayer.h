#pragma once

#include "Core/Defines.h"
#include "LayerStack/Layer.h"

namespace sl
{

class MouseScrollEvent;
class SceneViewportResizeEvent;

}

class CameraControllerLayer : public sl::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(sl::Event &event) override;

	virtual void BeginFrame() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void EndFrame() override;

private:
	void UpdateFPSMode(float deltaTime);
	void UpdateEditorMode(float deltaTime);

	bool OnMouseScroll(sl::MouseScrollEvent &event);
	bool OnSceneViewportResize(sl::SceneViewportResizeEvent &event);

	bool m_isRotating = false;
	bool m_isMoving = false;
};
