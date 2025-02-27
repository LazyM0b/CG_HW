#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include <wrl.h>

struct Vertex {
	DirectX::XMFLOAT4 location;
	DirectX::XMFLOAT4 color;
};

class TriangleComponent {
public:

	TriangleComponent();
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device);
	void Draw(ID3D11DeviceContext* context);

	std::vector <Vertex> points;
	std::vector <int> indeces;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	UINT strides;
	UINT offsets;
};