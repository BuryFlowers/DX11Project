#include "systemclass.h"

SystemClass::SystemClass()
{

	mInput = nullptr;
	mApplication = nullptr;

}

bool SystemClass::Init()
{

	int screenWidth = 0, screenHeight = 0;

	InitWindows(screenWidth, screenHeight);
	mInput = new InputClass();
	mInput->Init();
	mApplication = new ApplicationClass();
	return mApplication->Init(screenWidth, screenHeight, mHwnd);

}

void SystemClass::Shutdown()
{

	if (mApplication)
	{

		mApplication->Shutdown();
		delete mApplication;
		mApplication = nullptr;

	}

	if (mInput)
	{

		delete mInput;
		mInput = nullptr;

	}

	ShutdownWindows();

	return;

}

void SystemClass::Run()
{

	MSG msg;
	bool done = false;
	ZeroMemory(&msg, sizeof(MSG));

	while (!done)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}

		if (msg.message == WM_QUIT) done = true;
		else if (!Frame()) done = true;

	}

	return;

}

bool SystemClass::Frame()
{

	if (mInput->IsKeyDown(VK_ESCAPE)) return false;

	return mApplication->Frame();

}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{

	switch (umsg)
	{

		case WM_KEYDOWN:
		{

			mInput->KeyDown((unsigned int)wparam);
			return 0;

		}

		case WM_KEYUP:
		{

			mInput->KeyUp((unsigned int)wparam);
			return 0;

		}

		default:
		{

			return DefWindowProc(hwnd, umsg, wparam, lparam);

		}

		break;
	}

}

void SystemClass::InitWindows(int& screenWidth, int& screenHeight)
{

	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	ApplicationHandle = this;
	mHinstance = GetModuleHandle(nullptr);
	mApplicationName = L"DX11";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mHinstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = mApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{

		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;

	}

	else

	{

		screenWidth = 1600;
		screenHeight = 900;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}

	mHwnd = CreateWindowEx(WS_EX_APPWINDOW, mApplicationName, mApplicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, nullptr, nullptr, mHinstance, nullptr);

	ShowWindow(mHwnd, SW_SHOW);
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);

	ShowCursor(false);

	return;

}

void SystemClass::ShutdownWindows()
{

	ShowCursor(true);

	if (FULL_SCREEN) ChangeDisplaySettings(nullptr, 0);

	DestroyWindow(mHwnd);
	mHwnd = nullptr;

	UnregisterClass(mApplicationName, mHinstance);
	mHinstance = nullptr;

	ApplicationHandle = nullptr;

	return;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{

	switch (umessage)
	{

		case WM_DESTROY:
		{

			PostQuitMessage(0);
			return 0;

		}

		case WM_CLOSE:
		{

			PostQuitMessage(0);
			return 0;

		}

		default:
		{

			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);

		}

		break;

	}

}