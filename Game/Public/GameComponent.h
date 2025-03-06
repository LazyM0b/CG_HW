#pragma once

#include "TriangleComponent.h"
#include "DirectXCollision.h"
#include "SimpleMath.h"
#include "DisplayWin32.h"

using namespace DirectX;

enum MeshTypes
{
	Triangle,
	Square,
	Circle,
	Cube,
	Sphere,
	Mesh
};

class GameComponent: public TriangleComponent {
public:

	GameComponent();
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, MeshTypes type = Triangle);
	void Draw(ID3D11DeviceContext* context);
	void Reload();
	void Update();
	void DestroyResources();

	bool collisionEnabled = false;
	bool isMovable = false;
	bool isPlayerControllable = false;
	UINT pointsCnt = 100;
	MeshTypes type = Triangle;
	BoundingBox collider;
	float speed = 0.0f;
	SimpleMath::Vector3 velocity;
	SimpleMath::Vector3 scale;
	SimpleMath::Quaternion rotation;
	SimpleMath::Vector3 position;
	SimpleMath::Vector3 translation;
};