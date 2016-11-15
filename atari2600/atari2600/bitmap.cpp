#include "bitmap.h"

Bitmap::Bitmap()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
}

Bitmap::Bitmap(const Bitmap &)
{
}


Bitmap::~Bitmap()
{
}

bool Bitmap::Initialize(ID3D11Device * device, int screenWidth, int screenHeight, WCHAR *textureFilename, int bitmapWidth, int bitmapHeight)
{
	// Store the screen size
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the size in pixels that this bitmap should be rendered at.
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// Initialize the previous rendering position to -1
	m_previousPosX = -1;
	m_previousPosY = -1;

	if (!InitializeBuffers(device))
	{
		return false;
	}

	if (!LoadTexture(device, textureFilename))
	{
		return false;
	}

	return true;
}

void Bitmap::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	return;
}

bool Bitmap::Render(ID3D11DeviceContext * deviceContext, int x, int y)
{
	if (!UpdateBuffers(deviceContext, x, y))
	{
		return false;
	}
	RenderBuffers(deviceContext);

	return true;
}

int Bitmap::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView * Bitmap::GetTexture()
{
	return m_Texture->GetTexture();
}

// Here you either read a model and create the buffers from that data file or create them manually as in this time.
bool Bitmap::InitializeBuffers(ID3D11Device * device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	/// NOTE: We set the vertices to six since we are making a square out of two triangles, so six points are needed. The indices will be the same.

	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;
	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	//Create index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return	false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType)* m_vertexCount));

	for (i = 0; i < m_vertexCount; i++)
	{
		indices[i] = i;
	}

	//Set up the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Release arrays after buffers have been created and loaded.
	delete[]vertices;
	vertices = 0;
	delete[]indices;
	indices = 0;

	return true;
}

void Bitmap::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
	return;
}

bool Bitmap::UpdateBuffers(ID3D11DeviceContext * deviceContext, int x, int y)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	// if the position we are rendering this bitmap to has not changed then do not update the vertex buffer since it currently has the correct parameters.
	if ((x == m_previousPosX) && (y == m_previousPosY))
	{
		return true;
	}

	// if it has changed then update the position it is being rendered to.
	m_previousPosX = x;
	m_previousPosY = y;

	// Calculate the screen coordinates
	left = (float)((m_screenWidth / 2)* -1) + (float)x;
	right = left + (float)m_bitmapWidth;
	top = (float)((m_screenHeight / 2)) - (float)y;
	bottom = top - (float)m_bitmapHeight;

	// Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Load the vertex array with data.

	// First triangle.
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

	// Second triangle.
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[3].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[4].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*m_vertexCount));

	// Unlock the vertex buffer
	deviceContext->Unmap(m_vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

// Activate vertex and input buffers on the input assembler and define how buffers should be drawn 
void Bitmap::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	//Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//Set the index buffer to active in the input assembler so it can be renderede.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of primitive that should be rendered from this vertex, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}