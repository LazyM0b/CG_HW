#include "PongGame.h"
#include "FiveSquares.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

int main()
{
	HINSTANCE hinst = GetModuleHandle(nullptr);
	LPCWSTR applicationName = L"My3DApp";

	//FiveSquares
	/*FiveSquares CurrentGame(hinst, applicationName);

	CurrentGame.Initialize(5);*/
	//

	//Pong
	PongGame CurrentGame(hinst, applicationName);

	CurrentGame.Initialize(15);
	//

	CurrentGame.PrepareResources();

	CurrentGame.Run();
}