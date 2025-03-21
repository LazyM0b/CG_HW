#include "PongGame.h"
#include "FiveSquares.h"
#include "SolarSystem.h"
#include "Katamari.h"

int main()
{
	HINSTANCE hinst = GetModuleHandle(nullptr);
	LPCWSTR applicationName1 = L"Squares";
	LPCWSTR applicationName2 = L"Pong";
	LPCWSTR applicationName3 = L"SolarSys";
	LPCWSTR applicationName4 = L"Katamari";
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


	//Lab 3
	/*SolarSystem SolarSys(hinst, applicationName3);
	SolarSys.Initialize(214);

	SolarSys.PrepareResources();

	SolarSys.Run();*/

	//Lab 4
	Katamari Katamari(hinst, applicationName4);

	Katamari.Initialize(250);

	Katamari.PrepareResources();

	Katamari.Run();

}