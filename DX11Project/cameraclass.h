#pragma once
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <DirectXMath.h>

using namespace DirectX;

class CameraClass
{

public:

	CameraClass();
	CameraClass(const CameraClass&) = delete;
	~CameraClass() = default;

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:

	float mPositionX, mPositionY, mPositionZ;
	float mRotationX, mRotationY, mRotationZ;
	XMMATRIX mViewMatrix;

};

#endif // !_CAMERACLASS_H_
