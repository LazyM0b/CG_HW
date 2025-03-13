#include "Katamari.h"

Katamari::Katamari(HINSTANCE hinst, LPCTSTR hwindow) : Game(hinst, hwindow) {};

void Katamari::Initialize(UINT objCnt) {

	meshes.push_back(Cube);
	for (int i = 1; i < objCnt - 1; ++i) {
		meshes.push_back(Mesh);
	}
	meshes.push_back(Cube);

	Game::Initialize(objCnt);
	for (int i = 0; i < objCnt - 1; ++i)
		objects[i]->isMovable = true;
	objects[objCnt - 1]->isMovable = false;

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

		objects[i]->positionL = Matrix::CreateScale(objects[i]->scale);
		objects[i]->positionL *= Matrix::CreateFromYawPitchRoll(objects[i]->rotation.ToEuler());
		objects[i]->positionL *= Matrix::CreateTranslation(objects[i]->translation);

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

void Katamari::ResetGame()
{
	for (int i = 0; i < objects.size() - 1; ++i) {

	}
	objects[objects.size() - 1]->scale = Vector3(100000.0f, 0.1f, 100000.0f);

	camManager->SetViewMatrix(objects[0]->translation, Vector3(objects[0]->scale.x * 2, objects[0]->scale.x * 2, 0.0f), Quaternion::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), 0.0f));

	float aspectRatio = 16.0f / 9.0f;
	float nearPlane = 1000.0f;
	float farPlane = 25000.0f;
	projectionMatrix = Matrix::CreatePerspectiveFieldOfView(90.0f, aspectRatio, nearPlane, farPlane);
}
