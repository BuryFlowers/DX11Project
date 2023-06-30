#include "textureclass.h"

TextureClass::TextureClass()
{

	mTargaData = nullptr;
	mTexture = nullptr;
	mTextureView = nullptr;

}

bool TextureClass::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{

	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	if (!LoadTarga32Bit(filename)) return false;

	textureDesc.Height = mHeight;
	textureDesc.Width = mWidth;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &mTexture))) return false;

	rowPitch = (mWidth * 4) * sizeof(unsigned char);

	deviceContext->UpdateSubresource(mTexture, 0, nullptr, mTargaData, rowPitch, 0);

	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	if (FAILED(device->CreateShaderResourceView(mTexture, &srvDesc, &mTextureView))) return false;

	deviceContext->GenerateMips(mTextureView);
	
	delete[] mTargaData;
	mTargaData = nullptr;

	return true;

}

void TextureClass::Shutdown()
{

	if (mTextureView)
	{

		mTextureView->Release();
		mTextureView = nullptr;

	}

	if (mTexture)
	{

		mTexture->Release();
		mTexture = nullptr;

	}

	if (mTargaData)
	{

		delete[] mTargaData;
		mTargaData = nullptr;

	}

	return;

}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{

	return mTextureView;

}

bool TextureClass::LoadTarga32Bit(char* filename)
{

	int bpp, imageSize, index;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	if (fopen_s(&filePtr, filename, "rb") != 0) return false;

	if ((unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr) != 1) return false;

	mHeight = (int)targaFileHeader.height;
	mWidth = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	if (bpp == 32)
	{

		imageSize = mHeight * mWidth * 4;
		targaImage = new unsigned char[imageSize];

		if ((unsigned int)fread(targaImage, 1, imageSize, filePtr) != imageSize) return false;

		if (fclose(filePtr) != 0) return false;

		mTargaData = new unsigned char[imageSize];
		index = 0;
		int k = (mHeight * mWidth * 4) - (mWidth * 4);

		for (int j = 0; j < mHeight; j++)
		{

			for (int i = 0; i < mWidth; i++)
			{

				mTargaData[index + 0] = targaImage[k + 2];
				mTargaData[index + 1] = targaImage[k + 1];
				mTargaData[index + 2] = targaImage[k + 0];
				mTargaData[index + 3] = targaImage[k + 3];

				k += 4;
				index += 4;

			}

			k -= mWidth * 8;

		}

	}
	else if (bpp == 24)
	{

		imageSize = mHeight * mWidth * 3;
		targaImage = new unsigned char[mHeight * mWidth * 4];

		if ((unsigned int)fread(targaImage, 1, imageSize, filePtr) != imageSize) return false;

		if (fclose(filePtr) != 0) return false;

		mTargaData = new unsigned char[mHeight * mWidth * 4];
		index = 0;
		int k = (mHeight * mWidth * 3) - (mWidth * 3);

		for (int j = 0; j < mHeight; j++)
		{

			for (int i = 0; i < mWidth; i++)
			{

				mTargaData[index + 0] = targaImage[k + 2];
				mTargaData[index + 1] = targaImage[k + 1];
				mTargaData[index + 2] = targaImage[k + 0];
				mTargaData[index + 3] = 255;

				k += 3;
				index += 4;

			}

			k -= mWidth * 6;

		}

	}

	else return false;

	delete[] targaImage;
	targaImage = 0;

	return true;

}

int TextureClass::GetHeight()
{

	return mHeight;

}

int TextureClass::GetWidth()
{

	return mWidth;

}