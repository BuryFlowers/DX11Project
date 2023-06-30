#include "lightclass.h"

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{

	mDiffuseColor = XMFLOAT4(r, g, b, a);
	return;

}

void LightClass::SetDirection(float x, float y, float z)
{

	mDirection = XMFLOAT3(x, y, z);
	return;

}

XMFLOAT4 LightClass::GetDiffuseColor()
{

	return mDiffuseColor;

}

XMFLOAT3 LightClass::GetDirection()
{

	return mDirection;

}