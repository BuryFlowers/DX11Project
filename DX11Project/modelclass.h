#pragma once
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class ModelClass
{

public:

	ModelClass();
	ModelClass(const ModelClass&) = delete;
	~ModelClass() = default;

	bool Init(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:

	struct VertexType
	{

		XMFLOAT3 position;
		XMFLOAT4 color;

	};

	bool InitBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	ID3D11Buffer* mVertexBuffer, * mIndexBuffer;
	int mVertexCount, mIndexCount;

};

#endif // !_MODELCLASS_H_
