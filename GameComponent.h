#pragma once

#include "DisplayWin32.h"
#include "Game.h"
#include "ShadersComponent.h"
#include "TriangleComponent.h"

class GameComponent {
public:

	Game CurrentGame;
	ShadersComponent Shaders;
	TriangleComponent Triangles;
	DisplayWin32 Display;

	GameComponent();
	void Initialize();
	void Draw();
	void Reload();
	void Update();
	void DestroyResources();
};