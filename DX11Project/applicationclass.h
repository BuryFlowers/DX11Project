#pragma once

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

#include <stdio.h>
#include <fstream>
#include <iostream>
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"

///////////
//GLOBALS//
///////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class ApplicationClass
{

public:

	ApplicationClass();
	ApplicationClass(const ApplicationClass&) = delete;
	~ApplicationClass() = default;

	bool Init(int, int, HWND);
	void Shutdown();
	bool Frame();

private:

	bool Render();

private:

	D3DClass* mDirect3D;
	CameraClass* mCamera;
	ModelClass* mModel;
	ColorShaderClass* mColorShader;

};


#endif // !_APPLICATIONCLASS_H_
