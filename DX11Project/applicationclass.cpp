#include "applicationclass.h"

ApplicationClass::ApplicationClass()
{

	mDirect3D = nullptr;
	mCamera = nullptr;
	mModel = nullptr;
	mColorShader = nullptr;

}

bool ApplicationClass::Init(int screenWidth, int screenHeight, HWND hwnd)
{

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

	if (!mModel->Init(mDirect3D->GetDevice()))
	{

		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;

	}

	mColorShader = new ColorShaderClass();

	if (!mColorShader->Init(mDirect3D->GetDevice(), hwnd))
	{

		MessageBox(hwnd, L"Could not initialize the color shader object", L"Error", MB_OK);
		return false;

	}


	return true;

}

void ApplicationClass::Shutdown()
{

	if (mColorShader)
	{

		mColorShader->Shutdown();
		delete mColorShader;
		mColorShader = nullptr;

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

	return Render();

}

bool ApplicationClass::Render()
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	mDirect3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	mCamera->Render();

	mDirect3D->GetWorldMatrix(worldMatrix);
	mCamera->GetViewMatrix(viewMatrix);
	mDirect3D->GetProjectionMatrix(projectionMatrix);

	mModel->Render(mDirect3D->GetDeviceContext());

	if (!mColorShader->Render(mDirect3D->GetDeviceContext(), mModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix)) return false;

	mDirect3D->EndScene();
	return true;

}