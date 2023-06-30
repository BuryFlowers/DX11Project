#include "applicationclass.h"

ApplicationClass::ApplicationClass()
{

	mDirect3D = nullptr;
	mCamera = nullptr;
	mModel = nullptr;
	mLight = nullptr;
	mLightShader = nullptr;

}

bool ApplicationClass::Init(int screenWidth, int screenHeight, HWND hwnd)
{

	char modelFilename[128];
	char textureFilename[128];

	mDirect3D = new D3DClass();

	if (!mDirect3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{

		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;

	}

	char* cardname = new char[128]();
	int memory;
	std::ofstream fout;
	fout.open("CardInfo.txt");
	mDirect3D->GetVideoCardInfo(cardname, memory);
	fout << cardname << " : " << memory << "MB" << std::endl;
	fout.close();

	mCamera = new CameraClass();
	mCamera->SetPosition(0.0f, 0.0f, -5.0f);
	mCamera->SetRotation(0.0f, 5.0f, 5.0f);

	mModel = new ModelClass();

	strcpy_s(modelFilename, "./data/cube.txt");
	strcpy_s(textureFilename, "./data/stone01.tga");

	if (!mModel->Init(mDirect3D->GetDevice(), mDirect3D->GetDeviceContext(), modelFilename, textureFilename))
	{

		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;

	}

	mLightShader = new LightShaderClass();

	if (!mLightShader->Init(mDirect3D->GetDevice(), hwnd))
	{

		MessageBox(hwnd, L"Could not initialize the light shader object", L"Error", MB_OK);
		return false;

	}

	mLight = new LightClass();
	mLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	mLight->SetDirection(0.0f, 0.0f, 1.0f);

	return true;

}

void ApplicationClass::Shutdown()
{

	if (mLight)
	{

		delete mLight;
		mLight = 0;

	}

	if (mLightShader)
	{

		mLightShader->Shutdown();
		delete mLightShader;
		mLightShader = nullptr;

	}

	if (mModel)
	{

		mModel->Shutdown();
		delete mModel;
		mModel = nullptr;

	}

	if (mCamera)
	{

		delete mCamera;
		mCamera = nullptr;

	}

	if (mDirect3D)
	{

		mDirect3D->Shutdown();
		delete mDirect3D;
		mDirect3D = nullptr;

	}
	return;

}

bool ApplicationClass::Frame()
{

	static float rotation = 0.0f;

	rotation -= 0.00174532925f;
	if (rotation < 0.0f) rotation += XM_2PI;
	return Render(rotation);

}

bool ApplicationClass::Render(float rotation)
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	mDirect3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	mCamera->Render();

	mDirect3D->GetWorldMatrix(worldMatrix);
	mCamera->GetViewMatrix(viewMatrix);
	mDirect3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixRotationY(rotation);

	mModel->Render(mDirect3D->GetDeviceContext());

	if (!mLightShader->Render(mDirect3D->GetDeviceContext(), mModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, mModel->GetTexture(), mLight->GetDirection(), mLight->GetDiffuseColor())) return false;

	mDirect3D->EndScene();
	return true;

}