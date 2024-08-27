#include "Editor.h"

int main()
{
	EditorInitor initor{ "Slam Engine", 1920, 1080, sl::GraphicsBackend::OpenGL };
	Editor *pApp = new Editor{ std::move(initor) };

	pApp->Run();

	delete pApp;

	return 0;
}
