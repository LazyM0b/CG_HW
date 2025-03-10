#include "CameraManager.h"

CameraManager::CameraManager() {}

CameraManager::CameraManager(float dist)
{
	distance = dist;
	cameraPos = Vector3(dist, 0.0f, 0.0f);
	cameraRotation = Vector3(0.0f, 0.0f, 0.0f);
}

void CameraManager::SetViewMatrix(Vector3 objPos, float dist, Vector3 camRotation)
{
	objectPos = objPos;
	distance = dist;
	cameraRotation = camRotation;

	cameraPos = Vector3::Transform(Vector3(distance, distance, 0.0f), Matrix::CreateFromYawPitchRoll(camRotation)) + objectPos;

	viewMatrix = Matrix::CreateLookAt(cameraPos, objectPos, Vector3::Up);
}

void CameraManager::RotateCamera(Vector3 rotVector)
{
	cameraRotation += rotVector;
	SetViewMatrix(objectPos, distance, cameraRotation);
}
