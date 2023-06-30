#include "cameraclass.h"

CameraClass::CameraClass()
{

	mPositionX = 0.0f;
	mPositionY = 0.0f;
	mPositionZ = 0.0f;

	mRotationX = 0.0f;
	mRotationY = 0.0f;
	mRotationZ = 0.0f;

}

void CameraClass::SetPosition(float x, float y, float z)
{

	mPositionX = x;
	mPositionY = y;
	mPositionZ = z;
	return;

}

void CameraClass::SetRotation(float x, float y, float z)
{

	mRotationX = x;
	mRotationY = y;
	mRotationZ = z;
	return;

}

XMFLOAT3 CameraClass::GetPosition()
{

	return XMFLOAT3(mPositionX, mPositionY, mPositionZ);

}

XMFLOAT3 CameraClass::GetRotation()
{

	return XMFLOAT3(mRotationX, mRotationY, mRotationZ);

}

void CameraClass::Render()
{

	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	position.x = mPositionX;
	position.y = mPositionY;
	position.z = mPositionZ;

	positionVector = XMLoadFloat3(&position);

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = mRotationX * 0.0174532925f;
	yaw = mRotationY * 0.0174532925f;
	roll = mRotationZ * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	mViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;

}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{

	viewMatrix = mViewMatrix;
	return;

}