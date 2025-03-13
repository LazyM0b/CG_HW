#include "Katamari.h"

Katamari::Katamari(HINSTANCE hinst, LPCTSTR hwindow) : Game(hinst, hwindow) {};

void Katamari::Initialize(UINT objCnt) {

	meshes.push_back(Cube);
	meshes.push_back(Sphere);
	for (int i = 2; i < objCnt; ++i) {
		meshes.push_back(Mesh);
	}

	Game::Initialize(objCnt);

	objects[0]->isMovable = false;
	for (int i = 1; i < objCnt; ++i)
		objects[i]->isMovable = true;

	ResetGame();
}

void Katamari::Draw()
{
	float color[] = { 0.1, 0.1f, 0.2f, 1.0f };
	context->ClearRenderTargetView(renderView, color);
	shaders->Draw(context);

	D3D11_MAPPED_SUBRESOURCE res = {};

	camManager->UpdatePos(input);

	for (int i = 0; i < objects.size(); ++i) {

		if (objects[i]->isMovable == true) {
			objects[i]->positionL = Matrix::CreateScale(objects[i]->scale);
			objects[i]->positionL *= Matrix::CreateFromYawPitchRoll(objects[i]->rotation.ToEuler());
			objects[i]->positionL *= Matrix::CreateTranslation(objects[i]->translation);
		}

		objects[i]->positionW = objects[i]->positionL * camManager->viewMatrix;
		objects[i]->positionW *= projectionMatrix;
		objects[i]->positionW = objects[i]->positionW.Transpose();


		if (objects[i]->collisionEnabled)
			objects[i]->boxCollider.Center = objects[i]->translation;

		context->VSSetConstantBuffers(0, 1, &objects[i]->worldPosBuffer);
		context->Map(objects[i]->worldPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);


		auto dataPtr = reinterpret_cast<float*>(res.pData);
		memcpy(dataPtr, &objects[i]->positionW, sizeof(objects[i]->positionW));

		context->Unmap(objects[i]->worldPosBuffer, 0);

		/*context->VSSetConstantBuffers(1, 1, &objects[i]->localPosBuffer);
		context->Map(objects[i]->localPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);


		dataPtr = reinterpret_cast<float*>(res.pData);
		memcpy(dataPtr, &objects[i]->positionL, sizeof(objects[i]->positionL));

		context->Unmap(objects[i]->localPosBuffer, 0);*/

		objects[i]->Draw(context);
	}
}

void Katamari::ResetGame()
{
	objects[0]->scale = Vector3(100000.0f, 0.1f, 100000.0f);
	objects[1]->scale = Vector3(100.0f, 100.0f, 100.0f);
	objects[1]->translation = Vector3(0.0f, 100.0f, 0.0f);

	for (int i = 2; i < objects.size(); ++i) {
		objects[i]->scale = Vector3(50.0f, 50.0f, 50.0f);
		objects[i]->translation = Vector3(0.0f, 50.0f, 0.0f);
	}

	for (auto object : objects) {
		if (object->isMovable == false) {
			object->positionL = Matrix::CreateScale(object->scale);
			object->positionL *= Matrix::CreateFromYawPitchRoll(object->rotation.ToEuler());
			object->positionL *= Matrix::CreateTranslation(object->translation);
		}
	}

	camManager->SetViewMatrix(objects[1]->translation, Vector3(objects[1]->scale.x * 2.0f, objects[1]->scale.x * 2.0f, 0.0f), Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), 0.0f));

	float aspectRatio = 16.0f / 9.0f;
	float nearPlane = 10.0f;
	float farPlane = 25000.0f;
	projectionMatrix = Matrix::CreatePerspectiveFieldOfView(90.0f, aspectRatio, nearPlane, farPlane);
}
