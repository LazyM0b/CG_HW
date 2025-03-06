#pragma once

#include "Game.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class FiveSquares : public Game {
public:
	FiveSquares(HINSTANCE hinst, LPCTSTR hwindow);
	void Initialize(UINT objCnt);
	void Draw() override;
	void UpdateControllables();
};