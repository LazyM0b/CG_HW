#include "TriangleComponent.h"

TriangleComponent::TriangleComponent() {}

void TriangleComponent::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device) {
	points = {

		//fill viewport with grey
		{DirectX::XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f)},
		{DirectX::XMFLOAT4(-1.0f, -1.0f, 0.5f, 1.0f),DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f)},
		{DirectX::XMFLOAT4(1.0f, -1.0f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f)},
		{DirectX::XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f)},

		{DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
	};

	indeces = { 0,1,2, 1,0,3, 4,5,6, 5,4,7 };

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(Vertex) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = &points.front();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

	//index buffer initialization
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = &indeces.front();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

	strides = sizeof(Vertex);
	offsets = 0;
}

void TriangleComponent::Draw(ID3D11DeviceContext* context)
{
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
}
