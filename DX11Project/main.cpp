#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{

	SystemClass* System;

	System = new SystemClass();

	if (System->Init())
	{

		System->Run();

	}

	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;

}