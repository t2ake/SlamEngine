#pragma once

#include "Layer/Layer.h"

namespace sl
{

class MouseScrollEvent;
class SceneViewportResizeEvent;
class SceneViewportGetFocusEvent;
class SceneViewportLostFocusEvent;

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
	void UpdateMainCamera(float deltaTime);

	bool OnMouseScroll(sl::MouseScrollEvent &event);
	bool OnSceneViewportResize(sl::SceneViewportResizeEvent &event);
	bool OnSceneViewportGetFocus(sl::SceneViewportGetFocusEvent &event);
	bool OnSceneViewportLostFocus(sl::SceneViewportLostFocusEvent &event);
};
