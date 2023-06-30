#pragma once

#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "inputclass.h"
#include "applicationclass.h"

class SystemClass
{

public:

	SystemClass();
	SystemClass(const SystemClass&) = delete;
	~SystemClass() = default;

	bool Init();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:

	bool Frame();
	void InitWindows(int&, int&);
	void ShutdownWindows();

private:

	LPCWSTR mApplicationName;
	HINSTANCE mHinstance;
	HWND mHwnd;

	InputClass* mInput;
	ApplicationClass* mApplication;

};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif // !_SYSTEMCLASS_H_
