#pragma once

#include "Game.h"
#include "SimpleMath.h"
#include "random"

using namespace DirectX::SimpleMath;

class Katamari : public Game {
public:
	Katamari(HINSTANCE hinst, LPCTSTR hwindow);
	void Initialize(UINT objCnt);
	void Draw() override;
	void ResetGame();

};