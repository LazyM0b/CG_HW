#pragma once

#include "Game.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class Katamari : public Game {
public:
	Katamari(HINSTANCE hinst, LPCTSTR hwindow);
	void Initialize(UINT objCnt) override;
	void Draw() override;
	void ResetGame();

};