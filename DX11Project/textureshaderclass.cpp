#include "textureshaderclass.h"

TextureShaderClass::TextureShaderClass()
{

	mVertexShader = 0;
	mPixelShader = 0;
	mLayout = 0;
	mMatrixBuffer = 0;
	mSampleState = 0;

}

bool TextureShaderClass::Init(ID3D11Device* device, HWND hwnd)
{

	wchar_t vsFilename[128];
	wchar_t psFilename[128];

	if (wcscpy_s(vsFilename, 128, L"./texture.vs") != 0) return false;
	if (wcscpy_s(psFilename, 128, L"./texture.ps") != 0) return false;


	if (!InitShader(device, hwnd, vsFilename, psFilename)) return false;

	return true;

}

void TextureShaderClass::Shutdown()
{

	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;

}

bool TextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{

	// Set the shader parameters that it will use for rendering.
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture)) return false;

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;

}

bool TextureShaderClass::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{

	ID3D10Blob* erroeMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;	
	D3D11_SAMPLER_DESC samplerDesc;

	if (FAILED(D3DCompileFromFile(vsFilename, nullptr, nullptr, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &erroeMessage)))
	{

		if (erroeMessage) OutputShaderErrorMessage(erroeMessage, hwnd, vsFilename);
		else MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);

		return false;

	}

	if (FAILED(D3DCompileFromFile(psFilename, nullptr, nullptr, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &erroeMessage)))
	{

		if (erroeMessage) OutputShaderErrorMessage(erroeMessage, hwnd, psFilename);
		else MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);

		return false;

	}

	if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &mVertexShader))) return false;
	if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &mPixelShader))) return false;

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	if (FAILED(device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &mLayout))) return false;

	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&matrixBufferDesc, nullptr, &mMatrixBuffer))) return false;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(device->CreateSamplerState(&samplerDesc, &mSampleState))) return false;

	return true;

}

void TextureShaderClass::ShutdownShader()
{

	if (mSampleState)
	{

		mSampleState->Release();
		mSampleState = nullptr;

	}

	if (mMatrixBuffer)
	{

		mMatrixBuffer->Release();
		mMatrixBuffer = nullptr;

	}

	if (mLayout)
	{

		mLayout->Release();
		mLayout = 0;

	}

	if (mPixelShader)
	{

		mPixelShader->Release();
		mPixelShader = nullptr;

	}

	if (mVertexShader)
	{

		mVertexShader->Release();
		mVertexShader = nullptr;

	}

	return;

}

void TextureShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{

	char* compileErrors;
	unsigned long long bufferSize;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("ShaderError.txt");

	// Write out the error message.
	for (int i = 0; i < bufferSize; i++) fout << compileErrors[i];

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check ShaderError.txt for message.", shaderFilename, MB_OK);

	return;

}

bool TextureShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	if (FAILED(deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) return false;

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(mMatrixBuffer, 0);
	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;

}

void TextureShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{

	deviceContext->IASetInputLayout(mLayout);

	deviceContext->VSSetShader(mVertexShader, nullptr, 0);
	deviceContext->PSSetShader(mPixelShader, nullptr, 0);

	deviceContext->PSSetSamplers(0, 1, &mSampleState);

	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;

}