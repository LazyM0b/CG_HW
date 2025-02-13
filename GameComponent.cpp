#include "GameComponent.h"

GameComponent::GameComponent() {}

void GameComponent::Initialize() {
	DirectX::XMFLOAT4 points[12] = {
			DirectX::XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	};
	
	int indeces[] = { 0,1,2, 1,2,3, 2,3,4, 3,4,5 };

	LPCWSTR applicationName = L"My3DApp";
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	Display = DisplayWin32(applicationName, hInstance, 800, 800);

	CurrentGame.Initialize(Display);

	Triangles.Initialize(points, sizeof(points) / sizeof(DirectX::XMFLOAT4), indeces, sizeof(indeces) / sizeof(int));

	Shaders.Initialize(Display, CurrentGame.device, CurrentGame.context);

	CurrentGame.CreateBackBuffer();

	CurrentGame.PrepareResources(Triangles.points, Triangles.indeces);

	CurrentGame.Run(Shaders);
}