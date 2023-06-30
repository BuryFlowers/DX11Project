#pragma once
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>

#include "textureclass.h"

using namespace DirectX;

class ModelClass
{

public:

	ModelClass();
	ModelClass(const ModelClass&) = delete;
	~ModelClass() = default;

	bool Init(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:

	struct VertexType
	{

		XMFLOAT3 position;
		XMFLOAT2 texture;

	};

	bool InitBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	ID3D11Buffer* mVertexBuffer, * mIndexBuffer;
	int mVertexCount, mIndexCount;
	TextureClass* mTexture;

};

#endif // !_MODELCLASS_H_
