#pragma once

#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class CameraManager {
public:
	CameraManager();
	CameraManager(float dist);
	void SetViewMatrix(Vector3 objPos, float dist, Vector3 camRotation);
	void RotateCamera(Vector3 rotVector);

	float distance;
	Vector3 objectPos;
	Vector3 cameraPos;
	Vector3 cameraRotation;
	Matrix viewMatrix;
};