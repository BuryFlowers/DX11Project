#pragma once

#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_


#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;

class TextureShaderClass
{

public:

	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&) = delete;
	~TextureShaderClass() = default;

	bool Init(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

private:

	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

	struct MatrixBufferType
	{

		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;

	};

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;
	
	ID3D11SamplerState* mSampleState;

};


#endif // !_TEXTURESHADERCLASS_H_