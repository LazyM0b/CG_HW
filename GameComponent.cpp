#include "GameComponent.h"

GameComponent::GameComponent() {}

void GameComponent::Initialize() {
	DirectX::XMFLOAT4 points[16] = {

		//fill viewport with grey
		DirectX::XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, -1.0f, 0.5f, 1.0f),DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
		DirectX::XMFLOAT4(1.0f, -1.0f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),

		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};

	int indeces[] = { 0,1,2, 1,0,3, 4,5,6, 5,4,7 };

	HINSTANCE hinst = GetModuleHandle(nullptr);
	LPCWSTR applicationName = L"My3DApp";

	CurrentGame.InitWindow(hinst, applicationName);
	CurrentGame.Initialize();

	//

	Triangles.Initialize(points, sizeof(points) / sizeof(DirectX::XMFLOAT4), indeces, sizeof(indeces) / sizeof(int));

	Shaders.Initialize(CurrentGame.MainWindow());

	CurrentGame.PrepareResources(Triangles.points, Triangles.indeces, &Shaders);

	CurrentGame.Run(Shaders);
}