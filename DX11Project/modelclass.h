#pragma once
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_
#define _CRT_SECURE_NO_WARNINGS

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <vector>

#include "textureclass.h"

using namespace DirectX;

class ModelClass
{

public:

	ModelClass();
	ModelClass(const ModelClass&) = delete;
	~ModelClass() = default;

	bool Init(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:

	struct VertexType
	{

		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;

	};

	struct ModelType
	{

		float x, y, z;
		float tu, tv;
		float nx, ny, nz;

	};

	bool InitBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();
	bool LoadModel(char*);
	void ReleaseModel();
	bool ParseOBJFile(char*);

	ID3D11Buffer* mVertexBuffer, * mIndexBuffer;
	int mVertexCount, mIndexCount;
	TextureClass* mTexture;
	ModelType* mModel;

};

#endif // !_MODELCLASS_H_
