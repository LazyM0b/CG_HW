#include "TriangleComponent.h"

TriangleComponent::TriangleComponent() {}

void TriangleComponent::Initialize(DirectX::XMFLOAT4* pointsPtr, int pointsNum, int* indecesPtr, int indecesNum) {

	for (int i = 0; i < pointsNum; ++i)
		points.push_back(pointsPtr[i]);

	for (int i = 0; i < indecesNum; ++i)
		indeces.push_back(indecesPtr[i]);
}