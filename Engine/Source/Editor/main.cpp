#include "Editor.h"

int main()
{
	Editor *pApp = new Editor;

	pApp->Init();

	pApp->Update();

	pApp->Shutdown();

	delete pApp;

	return 0;
}
