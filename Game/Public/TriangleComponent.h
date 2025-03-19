#pragma once

#include <d3d11.h>
#include <vector>
#include <wrl.h>

#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

struct Vertex {
	Vector4 location;
	Vector4 color;
	Vector2 texCoord;
};

enum MeshTypes
{
	Strip,
	Triangle,
	Square,
	Circle,
	Cube,
	Sphere,
	Mesh
};

class TriangleComponent {
public:

	TriangleComponent();
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device);
	void DrawTriangle(ID3D11DeviceContext* context);
	void DrawLine(ID3D11DeviceContext* context);

	MeshTypes type;
	std::vector <Vertex> points;
	std::vector <int> indeces;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* worldPosBuffer;
	//ID3D11Buffer* localPosBuffer;
	UINT strides;
	UINT offsets;
};