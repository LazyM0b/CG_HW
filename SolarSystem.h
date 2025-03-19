#pragma once

#include "Game.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class SolarSystem : public Game {
public:
	SolarSystem(HINSTANCE hinst, LPCTSTR hwindow);
	void Initialize(UINT objCnt);
	void Draw() override;
	void ResetGame();
	bool SwitchPlanet();
};