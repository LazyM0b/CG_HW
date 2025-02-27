#include "GameComponent.h"

GameComponent::GameComponent() {}

void GameComponent::Initialize() {

	HINSTANCE hinst = GetModuleHandle(nullptr);
	LPCWSTR applicationName = L"My3DApp";

	//Init window component
	CurrentGame.InitWindow(hinst, applicationName);

	//Init game class
	CurrentGame.Initialize();

	CurrentGame.PrepareResources();

	//Init triangles component
	Triangles.Initialize(CurrentGame.device);

	//Init shaders component
	Shaders.Initialize(CurrentGame.MainWindow(), CurrentGame.device, CurrentGame.context);

	//Run game
	CurrentGame.Run(Shaders, Triangles);
}