#pragma once

#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include <DirectXMath.h>

using namespace DirectX;

class LightClass
{

public:

	LightClass() = default;
	LightClass(const LightClass&) = delete;
	~LightClass() = default;

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();

private:

	XMFLOAT4 mDiffuseColor;
	XMFLOAT3 mDirection;

};

#endif // !_LIGHTCLASS_H_
