#pragma once

#include "TriangleComponent.h"
#include "DirectXCollision.h"
#include "SimpleMath.h"
#include "DisplayWin32.h"

using namespace DirectX::SimpleMath;

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
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, MeshTypes type);
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, MeshTypes type, std::vector<Vector4> colors, UINT detailsLVL);
	void Draw(ID3D11DeviceContext* context);
	void Reload();
	void Update();
	void DestroyResources();
	void PointNormalize(Vertex& point);
	void SphereSubdivide(std::vector<Vertex>& points, std::vector<int>& indeces);
	Vector4 findCenter(const Vector4& point1, const Vector4& point2);
	int CheckForUnique(const std::vector<Vertex>& points, Vertex pointNew, int startInd);

	bool collisionEnabled = false;
	bool isMovable = false;
	bool isPlayerControllable = false;
	UINT pointsCnt;
	MeshTypes type = Triangle;
	DirectX::BoundingBox collider;
	float speed = 0.0f;
	float period;
	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
	Vector3 velocity;
	Matrix positionW;
	Matrix positionL;

	GameComponent* parent;
};