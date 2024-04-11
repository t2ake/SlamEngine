#include "Editor.h"

#include <memory>

int main()
{
	Editor *pApp = new Editor;

	pApp->Init();

	delete pApp;

	return 0;
}
