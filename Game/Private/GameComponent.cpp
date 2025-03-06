#include "GameComponent.h"

GameComponent::GameComponent() {}

void GameComponent::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, MeshTypes type) {

	this->type = type;

	switch (type)
	{
	case Triangle:
		points = {
		{XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT4(-1.0f, -1.0f, 0.5f, 1.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		};
		indeces = { 0,1,2 };
		break;
	case Square:
		points = {
		{XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT4(-1.0f, -1.0f, 0.5f, 1.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT4(1.0f, -1.0f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		};
		indeces = { 0,1,2, 1,0,3 };
		break;
	case Circle:
		points = { { XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) } };
		for (int i = 1; i < pointsCnt; ++i) {
			points.push_back(Vertex());
			points[i].location = XMFLOAT4(cos((i - 1) / (pointsCnt - 2.0f) * 6.28f), sin((i - 1) / (pointsCnt - 2.0f) * 6.28f), 0.5f, 1.0f);
			points[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indeces.push_back(0);
			indeces.push_back((i + 1) % pointsCnt);
			indeces.push_back(i);
		}
	default:
		break;
	}

	collider = BoundingBox(SimpleMath::Vector3::Zero, SimpleMath::Vector3::One);
	TriangleComponent::Initialize(device);
}

void GameComponent::Draw(ID3D11DeviceContext* context) {

	TriangleComponent::Draw(context);
	switch (type)
	{
	case Triangle:
		context->DrawIndexed(3, 0, 0);
		break;
	case Square:
		context->DrawIndexed(6, 0, 0);
		break;
	case Circle:
		context->DrawIndexed(pointsCnt * 3 - 3, 0, 0);
		break;
	default:
		break;
	}
}

void GameComponent::Reload() {
	scale = SimpleMath::Vector3::One;
	rotation = rotation.CreateFromYawPitchRoll(SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	translation = SimpleMath::Vector3::Zero;
}
