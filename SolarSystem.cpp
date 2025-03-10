#include "SolarSystem.h"

SolarSystem::SolarSystem(HINSTANCE hinst, LPCTSTR hwindow) : Game(hinst, hwindow) {};

void SolarSystem::Initialize(UINT objCnt) {
	for (int i = 0; i < objCnt; ++i)
		meshes.push_back(Sphere);

	Game::Initialize(objCnt);
	for (int i = 0; i < objCnt; ++i)
		objects[i]->isMovable = true;

	ResetGame();
}

void SolarSystem::Draw()
{
	float color[] = { 0.1, 0.1f, 0.1f, 1.0f };
	context->ClearRenderTargetView(renderView, color);
	shaders->Draw(context);

	D3D11_MAPPED_SUBRESOURCE res = {};
	SwitchPlanet();

	for (int i = 0; i < objects.size(); ++i) {
		if (i != 0)
			objects[i]->parent = objects[0];
		if (i == 4 || i == 8 || i == 9)
			objects[i]->parent = objects[i - 1];

		objects[i]->rotation = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), objects[i]->rotation.ToEuler().y + objects[i]->speed);

		//world position calculation
		if (objects[i]->parent != nullptr) {
			float dist = objects[i]->scale.x * 6;
			if (objects[i]->parent == objects[0])
				dist = objects[i]->scale.x * i * 3;
			objects[i]->translation = objects[i]->parent->translation + Vector3(cos(objects[i]->rotation.ToEuler().y + objects[i]->period) * dist, 0.0f, sin(objects[i]->rotation.ToEuler().y + objects[i]->period) * dist);
			//objects[i]->positionW = Matrix::CreateTranslation(objects[i]->parent->translation);
			//objects[i]->positionW = objects[i]->positionW.Transpose();
			objects[i]->positionL = Matrix::CreateScale(objects[i]->scale);
			objects[i]->positionL *= Matrix::CreateFromYawPitchRoll(objects[i]->rotation.ToEuler() + objects[i]->parent->rotation.ToEuler());
			objects[i]->positionL *= Matrix::CreateTranslation(objects[i]->translation);
			//objects[i]->positionL += objects[i]->positionW;
		}
		else {
			objects[i]->positionL = Matrix::CreateScale(objects[i]->scale);
			objects[i]->positionL *= Matrix::CreateFromYawPitchRoll(objects[i]->rotation.ToEuler());
			objects[i]->positionL *= Matrix::CreateTranslation(objects[i]->translation);
		}
		
		if (i == planetToTrack) {
			camManager->RotateCamera(Vector3(0.0f, 0.00714f, 0.0f));
			if (objects[i]->parent != nullptr) {
				Vector3 translation;
				Quaternion tmp;
				objects[planetToTrack]->positionL.Decompose(translation, tmp, translation);
				camManager->SetViewMatrix(translation, objects[planetToTrack]->scale.x * 2, camManager->cameraRotation);
			}
		}

		//projection view calculation
		objects[i]->positionL *= camManager->viewMatrix;
		objects[i]->positionL *= projectionMatrix;
		objects[i]->positionL = objects[i]->positionL.Transpose();


		if (objects[i]->collisionEnabled)
			objects[i]->collider.Center = objects[i]->translation;

		context->VSSetConstantBuffers(0, 1, &objects[i]->worldPosBuffer);
		context->Map(objects[i]->worldPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);


		auto dataPtr = reinterpret_cast<float*>(res.pData);
		memcpy(dataPtr, &objects[i]->positionW, sizeof(objects[i]->positionW));

		context->Unmap(objects[i]->worldPosBuffer, 0);

		context->VSSetConstantBuffers(1, 1, &objects[i]->localPosBuffer);
		context->Map(objects[i]->localPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);


		dataPtr = reinterpret_cast<float*>(res.pData);
		memcpy(dataPtr, &objects[i]->positionL, sizeof(objects[i]->positionL));

		context->Unmap(objects[i]->localPosBuffer, 0);

		objects[i]->Draw(context);
	}
}

void SolarSystem::ResetGame()
{
	//Initialize planets scales
	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->scale = Vector3((i + 5.0f) * 100.0f, (i + 5.0f) * 100.0f, (i + 5.0f) * 100.0f);
		objects[i]->rotation = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), 0.0f);
		objects[i]->translation = Vector3(i * 7500.0f, 0.0f, 0.0f);
		objects[i]->period = rand() % 600 / 100.0f;
		objects[i]->speed = rand() % 100 / 10000.0f;
	}

	camManager->SetViewMatrix(objects[0]->translation, objects[0]->scale.x * 2, Vector3(0.0f, 0.0f, 0.0f));
	
	float aspectRatio = 9.0f / 9.0f;
	float nearPlane = 1000.0f;
	float farPlane = 250000.0f;
	projectionMatrix = Matrix::CreatePerspectiveFieldOfView(90.0f, aspectRatio, nearPlane, farPlane);
}

void SolarSystem::SwitchPlanet() {
	int keyNum = 0;
	if (input->IsKeyDown(Keys::D0)) {
		keyNum = int(Keys::D0);
		}
	else if (input->IsKeyDown(Keys::D1)) {
		keyNum = int(Keys::D1);
	}
	else if (input->IsKeyDown(Keys::D2)) {
		keyNum = int(Keys::D2);
	}
	else if (input->IsKeyDown(Keys::D3)) {
		keyNum = int(Keys::D3);
	}
	else if (input->IsKeyDown(Keys::D4)) {
		keyNum = int(Keys::D4);
	}
	else if (input->IsKeyDown(Keys::D5)) {
		keyNum = int(Keys::D5);
	}
	else if (input->IsKeyDown(Keys::D6)) {
		keyNum = int(Keys::D6);
	}
	else if (input->IsKeyDown(Keys::D7)) {
		keyNum = int(Keys::D7);
	}
	else if (input->IsKeyDown(Keys::D8)) {
		keyNum = int(Keys::D8);
	}
	else if (input->IsKeyDown(Keys::D9)) {
		keyNum = int(Keys::D9);
	}
	else if (input->IsKeyDown(Keys::Space)) {
		keyNum = -1;
	}

	if (keyNum == -1) {
		planetToTrack = -1;
		camManager->SetViewMatrix(Vector3::Zero, 2500, camManager->cameraRotation);
	}
	else if (keyNum && keyNum - 48 != planetToTrack) {
		planetToTrack = keyNum - 48;
		camManager->SetViewMatrix(objects[planetToTrack]->translation, objects[planetToTrack]->scale.x * 2, camManager->cameraRotation);
	}
}
