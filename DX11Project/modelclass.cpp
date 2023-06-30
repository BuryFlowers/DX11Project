#include "modelclass.h"

ModelClass::ModelClass()
{

	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;
	mTexture = nullptr;
	mModel = nullptr;

}

bool ModelClass::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename)
{

	return LoadModel(modelFilename) && LoadTexture(device, deviceContext, textureFilename) && InitBuffers(device);

}

void ModelClass::Shutdown()
{

	ReleaseModel();

	ReleaseTexture();

	ShutdownBuffers();
	return;

}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{

	RenderBuffers(deviceContext);
	return;

}

int ModelClass::GetIndexCount()
{

	return mIndexCount;

}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{

	return mTexture->GetTexture();

}

bool ModelClass::InitBuffers(ID3D11Device* device)
{

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertices = new VertexType[mVertexCount];
	if (!vertices) return false;

	indices = new unsigned long[mIndexCount];
	if (!indices) return false;

	for (int i = 0; i < mVertexCount; i++)
	{

		vertices[i].position = XMFLOAT3(mModel[i].x, mModel[i].y, mModel[i].z);
		vertices[i].texture = XMFLOAT2(mModel[i].tu, mModel[i].tv);
		vertices[i].normal = XMFLOAT3(mModel[i].nx, mModel[i].ny, mModel[i].nz);

	}

	for (int i = 0; i < mIndexCount; i++) indices[i] = i;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer))) return false;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer))) return false;

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;

}

void ModelClass::ShutdownBuffers()
{
	if (mIndexBuffer)
	{

		mIndexBuffer->Release();
		mIndexBuffer = 0;

	}

	if (mVertexBuffer)
	{

		mVertexBuffer->Release();
		mVertexBuffer = 0;

	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{

	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;

}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{

	mTexture = new TextureClass();

	if (!mTexture->Init(device, deviceContext, filename)) return false;

}

void ModelClass::ReleaseTexture()
{

	if (mTexture)
	{

		mTexture->Shutdown();
		delete mTexture;
		mTexture = 0;

	}

	return;

}

bool ModelClass::LoadModel(char* filename)
{

	if (strstr(filename, ".obj") != nullptr) return ParseOBJFile(filename);

	std::ifstream fin;
	char input;

	fin.open(filename);

	if (fin.fail()) return false;

	fin.get(input);
	while (input != ':') fin.get(input);

	fin >> mVertexCount;
	mIndexCount = mVertexCount;

	mModel = new ModelType[mVertexCount];

	fin.get(input);
	while (input != ':') fin.get(input);
	fin.get(input);fin.get(input);

	for (int i = 0; i < mVertexCount; i++)
	{

		fin >> mModel[i].x >> mModel[i].y >> mModel[i].z;
		fin >> mModel[i].tu >> mModel[i].tv;
		fin >> mModel[i].nx >> mModel[i].ny >> mModel[i].nz;

	}

	fin.close();

	return true;

}

bool ModelClass::ParseOBJFile(char* filename)
{

	std::vector<XMFLOAT3> positionQueue;
	std::vector<XMFLOAT2> uvQueue;
	std::vector<XMFLOAT3> normalQueue;
	std::vector<ModelType> vertexQueue;

	//read the file
	std::ifstream objFile(filename);

	if (objFile.fail()) return false;

	//set max length of every line
	const int MAX_LENGTH = 1024;
	char data[MAX_LENGTH] = { 0 };

	while (objFile.getline(data, MAX_LENGTH)) {

		char type[MAX_LENGTH] = { 0 };
		//get the line's first word
		if (sscanf(data, "%s", type) != 1) continue;

		//if the word is 'v', this line will give the pisition of a vertex
		if (strlen(type) == 1 && type[0] == 'v') {

			XMFLOAT3 p;
			//read the position
			if (sscanf(data, "%s %f %f %f", type, &p.x, &p.y, &p.z) != 4) {

				printf("[Error] can't read potision from obj file!\n");
				exit(0);

			}

			//push it to queue
			positionQueue.push_back(p);

		}

		//if the word is 'vn', this line will give the normal of a vertex
		else if (strlen(type) == 2 && type[0] == 'v' && type[1] == 'n') {

			XMFLOAT3 n;
			//read the normal
			if (sscanf(data, "%s %f %f %f", type, &n.x, &n.y, &n.z) != 4) {

				printf("[Error] can't read normal from obj file!\n");
				exit(0);

			}

			//push it to queue
			normalQueue.push_back(n);

		}

		//if the word is 'vt', this line will give the texture coordinatess of a vertex
		else if (strlen(type) == 2 && type[0] == 'v' && type[1] == 't') {

			XMFLOAT2 t;
			//read the uv
			if (sscanf(data, "%s %f %f", type, &t.x, &t.y) != 3) {

				printf("[Error] can't read uvs from obj file!\n");
				exit(0);

			}

			//push it to queue
			uvQueue.push_back(t);

		}

		//if the word is 'f', this line will give the indices of a triangle
		else if (strlen(type) == 1 && type[0] == 'f') {

			int p[3] = { -1, -1, -1 };
			int n[3] = { -1, -1, -1 };
			int t[3] = { -1, -1, -1 };
			char s[3][MAX_LENGTH];
			//read three strings
			if (sscanf(data, "%s %s %s %s", type, s[0], s[1], s[2]) != 4) {

				printf("[Error] can't read indices from obj file!\n");
				exit(0);

			}

			else for (int i = 0; i < 3; i++) {

				//get the position index
				int tmp = 0, j = 0;
				while (s[i][j] != '/' && s[i][j] != ' ' && j < strlen(s[i])) {

					tmp *= 10;
					tmp += s[i][j] - '0';
					j++;

				}

				p[i] = tmp - 1;

				//get the normal index
				j++;
				tmp = 0;
				while (s[i][j] != '/' && s[i][j] != ' ' && j < strlen(s[i])) {

					tmp *= 10;
					tmp += s[i][j] - '0';
					j++;

				}

				if (s[i][j] == '/' && j < strlen(s[i])) n[i] = tmp - 1, j++;

				//get the uv index
				tmp = 0;
				while (s[i][j] != '/' && s[i][j] != ' ' && j < strlen(s[i])) {

					tmp *= 10;
					tmp += s[i][j] - '0';
					j++;

				}

				if (s[i][j] == '/' && j < strlen(s[i])) t[i] = tmp - 1, j++;
				tmp = 0;

			}

			//push it to the queue

			for (int i = 0; i < 3; i++)
			{

				ModelType vertex;

				if (p[i] != -1)
				{

					vertex.x = positionQueue[p[i]].x;
					vertex.y = positionQueue[p[i]].y;
					vertex.z = positionQueue[p[i]].z;

				}
				else vertex.x = vertex.y = vertex.z = 0.0f;

				if (t[i] != -1)
				{

					vertex.tu = uvQueue[t[i]].x;
					vertex.tv = uvQueue[t[i]].y;

				}
				else vertex.tu = vertex.tv = 0.0f;

				if (n[i] != -1)
				{

					vertex.nx = normalQueue[n[i]].x;
					vertex.ny = normalQueue[n[i]].y;
					vertex.nz = normalQueue[n[i]].z;

				}
				else vertex.nx = vertex.ny = vertex.nz = 0.0f;

				vertexQueue.push_back(vertex);

			}

		}

	}

	mVertexCount = (int)vertexQueue.size();
	mIndexCount = mVertexCount;
	mModel = new ModelType[mVertexCount];
	for (int i = 0; i < mVertexCount; i++) mModel[i] = vertexQueue[i];
	printf("[Success] an obj file has been loaded!\n");

	return true;

}

void ModelClass::ReleaseModel()
{

	if (mModel)
	{

		delete[] mModel;
		mModel = nullptr;

	}

	return;

}