#include "CameraManager.h"

CameraManager::CameraManager() {}

CameraManager::CameraManager(float dist)
{
	distance = dist;
	cameraPos = Vector3(dist, dist, 0.0f);
	cameraOffset = Vector3::Forward;
	cameraRotation = Quaternion::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), 0.0f);
}

void CameraManager::SetViewMatrix(Vector3 objPos, Vector3 camPos, Quaternion camRotation)
{
	objectPos = objPos;
	distance = Vector3::Distance(camPos, objPos);
	cameraRotation = camRotation;

	cameraPos = Vector3::Transform(camPos, Matrix::CreateFromQuaternion(cameraRotation)) + objectPos;

	viewMatrix = Matrix::CreateLookAt(cameraPos, objectPos, Vector3::Up);
}

void CameraManager::RotateCamera(Quaternion rotVector)
{
	cameraRotation *= rotVector;
}

void CameraManager::UpdatePos(InputDevice* input)
{
	if (input->MouseWheelDelta != 0) {
		camMoveSpeed = camMoveSpeed + input->MouseWheelDelta * 0.1f;
		if (camMoveSpeed < 0.0f)
			camMoveSpeed = 0.0f;
	}

	forwardVector = objectPos - cameraPos;
	forwardVector.Normalize();

	//if (input->MouseOffset.x)
	//RotateCamera(Quaternion::CreateFromAxisAngle(Vector3(0.0, 1.0, 0.0), acos(input->MouseOffset.x)));

	//if (input->MouseOffset.y)
	//RotateCamera(Quaternion::CreateFromAxisAngle(Vector3(1.0, 0.0, 0.0), asin(input->MouseOffset.y)));

	//float norm = sqrt(pow(input->MouseOffset.x, 2) + pow(input->MouseOffset.y, 2));

	/*if (input->MouseOffset.x > 0 || input->MouseOffset.y > 0) {
		cameraRotation *= tmp;
		cameraRotation *= tmp1;
	}*/
	//else
		//cameraRotation = Quaternion::CreateFromAxisAngle(Vector3::Zero, 0.0f);


	/*if (input->MouseOffset.y > 0) 
		objectPos.y += sin(input->MousePosition.y) * distance;*/

	if (input->MouseOffset.x) {
		cursorPos.x += input->MouseOffset.x * mouseMoveSpeed;
		cameraOffset.x = cos((cursorPos.x * 6.28f) / 800.0f);
		cameraOffset.z = sin((cursorPos.x * 6.28f) / 800.0f);
	}
	if (input->MouseOffset.y) {
		cursorPos.y -= input->MouseOffset.y * 5;
		if (cursorPos.y > 800)
			cursorPos.y = 800;
		else if (cursorPos.y < -800)
			cursorPos.y = -800;
		cameraOffset.y = sin(-1 * (cursorPos.y * 1.57f) / 800);
	}
	input->MouseOffset = Vector2::Zero;

	float moveSpeed = camMoveSpeed;

	if (input->IsKeyDown(Keys::LeftShift))
		moveSpeed *= 2;

	if (input->IsKeyDown(Keys::W)) {
		objectPos += forwardVector * moveSpeed;
	}
	if (input->IsKeyDown(Keys::S)) {
		objectPos += forwardVector * -1.0f * moveSpeed;
	}
	if (input->IsKeyDown(Keys::A)) {
		objectPos.x += cos(asin(1.0f) - forwardVector.z) * moveSpeed;
		objectPos.z += sin(acos(1.0f) - forwardVector.x) * moveSpeed;
	}
	if (input->IsKeyDown(Keys::D)) {
		objectPos.x += cos(asin(-1.0f) - forwardVector.z) * moveSpeed;
		objectPos.z += sin(acos(-1.0f) - forwardVector.x) * moveSpeed;
	}
	if (input->IsKeyDown(Keys::Q)) {
		objectPos.y += moveSpeed;
	}
	if (input->IsKeyDown(Keys::E)) {
		objectPos.y += moveSpeed * -1;
	}

	SetViewMatrix(objectPos, cameraOffset, cameraRotation);
}

