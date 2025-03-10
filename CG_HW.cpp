#include "PongGame.h"
#include "FiveSquares.h"
#include "SolarSystem.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

int main()
{
	HINSTANCE hinst = GetModuleHandle(nullptr);
	LPCWSTR applicationName1 = L"Squares";
	LPCWSTR applicationName2 = L"Pong";
	LPCWSTR applicationName3 = L"SolarSys";
	UINT programNum;

	/*std::cout << "Choose the program, that you want to be executed:\n1. Just 5 squares with wasd control and switch on digits\n2. Pong game\n3. Solar system model (not actual solar system) \nProgram: ";
	std::cin >> programNum;*/

	//FiveSquares
	FiveSquares Squares(hinst, applicationName1);

	//

	//Pong
	PongGame Pong(hinst, applicationName2);

	//

	//SolarSystem

	/*std::cout << "Write spheres LOD: ";

	std::cin >> CurrentGame.LOD;*/

	SolarSystem SolarSys(hinst, applicationName3);

	//

	/*switch (programNum)
	{
	case 1:
		Squares.Initialize(5);

		Squares.PrepareResources();

		Squares.Run();
		break;
	case 2:
		Pong.Initialize(15);

		Pong.PrepareResources();

		Pong.Run();
		break;
	case 3:
		SolarSys.Initialize(14);

		SolarSys.PrepareResources();

		SolarSys.Run();
		break;
	default:
		break;
	}*/

	SolarSys.Initialize(14);

	SolarSys.PrepareResources();

	SolarSys.Run();

}