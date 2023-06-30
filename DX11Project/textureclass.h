#pragma once

#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
#include <stdio.h>

class TextureClass
{

public:

	TextureClass();
	TextureClass(const TextureClass&) = delete;
	~TextureClass() = default;

	bool Init(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	int GetWidth();
	int GetHeight();

private:

	bool LoadTarga32Bit(char*);
	
	struct TargaHeader
	{

		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;

	};

	unsigned char* mTargaData;
	ID3D11Texture2D* mTexture;
	ID3D11ShaderResourceView* mTextureView;
	int mWidth, mHeight;

};

#endif // !_TEXTURECLASS_H_
