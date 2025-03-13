#include "SolarSystem.h"

SolarSystem::SolarSystem(HINSTANCE hinst, LPCTSTR hwindow) : Game(hinst, hwindow) {};

void SolarSystem::Initialize(UINT objCnt) {
	for (int i = 0; i < objCnt - 200; ++i)
		meshes.push_back(Sphere);
	for (int i = objCnt - 200; i < objCnt; ++i)
		meshes.push_back(Cube);

	Game::Initialize(objCnt);
	for (int i = 0; i < objCnt - 200; ++i)
		objects[i]->isMovable = true;

	ResetGame();
}

void SolarSystem::Draw()
{

	float color[] = { 0.1, 0.1f, 0.2f, 1.0f };
	context->ClearRenderTargetView(renderView, color);
	shaders->Draw(context);

	D3D11_MAPPED_SUBRESOURCE res = {};
	SwitchPlanet();

	if (planetToTrack == -1) {
		camManager->UpdatePos(input);
	}
	/*std::cout << camManager->cameraRotation.x << "  " << camManager->cameraRotation.y << "  " << camManager->cameraRotation.z << "  " << "  " <<
		cos(-1 * (input->MousePosition.x * 6.28f) / 1600.0f) << std::endl;*/
	
	//std::cout << cos(-1 * (input->MousePosition.x * 6.28f) / 1600.0f) << std::endl;

	//std::cout << camManager->cameraPos.x << "  " << camManager->cameraPos.y << "  " << camManager->cameraPos.z << "    " << camManager->objectPos.x + cos(-1 * (input->MousePosition.x * 6.28f) / 1600.0f) << std::endl;
	//std::cout << camManager->objectPos.x << "  " << camManager->objectPos.y << "  " << camManager->objectPos.z << "   " << camManager->distance << std::endl << std::endl;

	/*std::cout << input->MouseOffset.x << "   " << input->MouseOffset.y << std::endl;
	std::cout << camManager->cursorPos.x << "   " << camManager->cursorPos.y << std::endl;*/

	//std::cout << (sin(-1 * (camManager->cursorPos.y * 6.28f) / (800.0f / camManager->mouseMoveSpeed))) << "  " << camManager->cursorPos.y << std::endl;

	for (int i = 0; i < objects.size(); ++i) {

		//planets rotation
		objects[i]->rotation = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), objects[i]->rotation.ToEuler().y + objects[i]->speed);

		//world position calculation
		if (objects[i]->parent != nullptr) 
			//orbital movement
			objects[i]->translation = objects[i]->parent->translation + Vector3(cos(objects[i]->rotation.ToEuler().y + objects[i]->period) * objects[i]->distanceToParent, 0.0f, sin(objects[i]->rotation.ToEuler().y + objects[i]->period) * objects[i]->distanceToParent);

		objects[i]->positionL = Matrix::CreateScale(objects[i]->scale);
		Vector3 rotation = objects[i]->rotation.ToEuler();
		if (objects[i]->parent != nullptr)
			rotation += objects[i]->parent->rotation.ToEuler();
		objects[i]->positionL *= Matrix::CreateFromYawPitchRoll(rotation);
		objects[i]->positionL *= Matrix::CreateTranslation(objects[i]->translation);

		if (i == planetToTrack) {

			//camera rotation
			camManager->RotateCamera(Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), 0.00714f));

			if (objects[i]->parent != nullptr) {
				Vector3 translation;
				Quaternion tmp;
				objects[planetToTrack]->positionL.Decompose(translation, tmp, translation);
				camManager->SetViewMatrix(translation, Vector3(objects[planetToTrack]->scale.x * 2, objects[planetToTrack]->scale.x * 2, 0.0f), camManager->cameraRotation);
			}
		}

		//projection view calculation
		objects[i]->positionL *= camManager->viewMatrix;
		objects[i]->positionL *= projectionMatrix;
		objects[i]->positionL = objects[i]->positionL.Transpose();


		if (objects[i]->collisionEnabled)
			objects[i]->boxCollider.Center = objects[i]->translation;

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
		if (i < objects.size() - 200) {
			objects[i]->scale = Vector3((i + 5.0f) * 100.0f, (i + 5.0f) * 100.0f, (i + 5.0f) * 100.0f);
			objects[i]->rotation = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), 0.0f);
			objects[i]->translation = Vector3(i * 7500.0f, 2000.0f, 0.0f);
			objects[i]->period = rand() % 600 / 100.0f;
			objects[i]->speed = rand() % 100 / 10000.0f;

			if (i != 0) {
				objects[i]->parent = objects[0];
				objects[i]->distanceToParent = objects[i]->scale.x * i * 3;
			}

			if (i == 4 || i == 8 || i == 9) {
				objects[i]->parent = objects[i - 1];
				objects[i]->speed *= rand() % 5 + 2;
				objects[i]->distanceToParent = objects[i]->scale.x * (rand() % 3 + 3.0f);
			}
		}
		else {
			if (i < objects.size() - 100) {
				objects[i]->scale = Vector3(100000.0f, 0.1f, 10.0f);
				objects[i]->translation = Vector3(0.0f, 0.0f, (i - (int)objects.size() + 150.0f) * 1000.0f);
			}
			else {
				objects[i]->scale = Vector3(10.0f, 0.1f, 100000.0f);
				objects[i]->translation = Vector3((i - (int)objects.size() + 50.0f) * 1000.0f, 0.0f, 0.0f);
			}
		}
	}

	camManager->SetViewMatrix(objects[0]->translation, Vector3(objects[planetToTrack]->scale.x * 2, objects[planetToTrack]->scale.x * 2, 0.0f), Quaternion::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), 0.0f));
	
	float aspectRatio = 16.0f / 9.0f;
	float nearPlane = 1000.0f;
	float farPlane = 250000.0f;
	projectionMatrix = Matrix::CreatePerspectiveFieldOfView(90.0f, aspectRatio, nearPlane, farPlane);
}

void SolarSystem::SwitchPlanet() {
	int keyNum = 0;
	if (input->IsKeyDown(Keys::D0)) {
		keyNum = int(Keys::D0);
		input->RemovePressedKey(Keys::D0);
		}
	else if (input->IsKeyDown(Keys::D1)) {
		keyNum = int(Keys::D1);
		input->RemovePressedKey(Keys::D1);
	}
	else if (input->IsKeyDown(Keys::D2)) {
		keyNum = int(Keys::D2);
		input->RemovePressedKey(Keys::D2);
	}
	else if (input->IsKeyDown(Keys::D3)) {
		keyNum = int(Keys::D3);
		input->RemovePressedKey(Keys::D3);
	}
	else if (input->IsKeyDown(Keys::D4)) {
		keyNum = int(Keys::D4);
		input->RemovePressedKey(Keys::D4);
	}
	else if (input->IsKeyDown(Keys::D5)) {
		keyNum = int(Keys::D5);
		input->RemovePressedKey(Keys::D5);
	}
	else if (input->IsKeyDown(Keys::D6)) {
		keyNum = int(Keys::D6);
		input->RemovePressedKey(Keys::D6);
	}
	else if (input->IsKeyDown(Keys::D7)) {
		keyNum = int(Keys::D7);
		input->RemovePressedKey(Keys::D7);
	}
	else if (input->IsKeyDown(Keys::D8)) {
		keyNum = int(Keys::D8);
		input->RemovePressedKey(Keys::D8);
	}
	else if (input->IsKeyDown(Keys::D9)) {
		keyNum = int(Keys::D9);
		input->RemovePressedKey(Keys::D9);
	}
	else if (input->IsKeyDown(Keys::Space)) {
		keyNum = -1;
		input->RemovePressedKey(Keys::Space);
	}

	if (keyNum == -1) {
		planetToTrack = -1;
		camManager->SetViewMatrix(Vector3(0.0f, 2000.0f, -2000.0f), Vector3::Forward, Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), asin(0.0f)));
	}
	else if (keyNum > 0 && keyNum - 48 != planetToTrack) {
		planetToTrack = keyNum - 48;
		camManager->SetViewMatrix(objects[planetToTrack]->translation, Vector3(objects[planetToTrack]->scale.x * 2, objects[planetToTrack]->scale.x * 2, 0.0f), camManager->cameraRotation);
	}
}
