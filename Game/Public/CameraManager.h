#pragma once

#include "SimpleMath.h"
#include "InputDevice.h"

using namespace DirectX::SimpleMath;

class CameraManager {
public:
	CameraManager();
	CameraManager(float dist);
	void SetViewMatrix(Vector3 objPos, Vector3 camPos, Quaternion camRotation);
	void RotateCamera(Quaternion rotVector);
	void UpdatePos(InputDevice* input);

	float distance;
	float camMoveSpeed = 20.1f; 
	float camRotationSpeed = 0.1f;
	float mouseMoveSpeed = 1.0f;
	Vector3 forwardVector;
	Vector3 objectPos;
	Vector3 cameraPos;
	Vector3 cameraOffset;
	Vector3 cursorPos;
	Quaternion cameraRotation;
	Matrix viewMatrix;
};