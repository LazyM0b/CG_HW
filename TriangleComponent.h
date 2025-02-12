#pragma once

#include <DirectXMath.h>
#include <vector>

class TriangleComponent {
public:

	TriangleComponent();
	void Initialize(DirectX::XMFLOAT4* pointsPtr, int pointsNum, int* indecesPtr, int indecesNum);

	std::vector <DirectX::XMFLOAT4> points;
	std::vector <int> indeces;
};