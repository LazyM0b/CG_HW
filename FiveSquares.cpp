#include "FiveSquares.h"

FiveSquares::FiveSquares(HINSTANCE hinst, LPCTSTR hwindow) : Game(hinst, hwindow) {}

void FiveSquares::Initialize(UINT objCnt)
{
	for (int i = 0; i < objCnt; ++i) {
		meshes.push_back(Square);
	}
	Game::Initialize(objCnt);

	for (int i = 0; i < objCnt; ++i) {
		objects[i]->isMovable = true;
		objects[i]->scale = Vector3(0.05f, 0.05f, 0.0f);
		objects[i]->translation = Vector3(i * 0.4f - 0.8f, i * 0.4f - 0.8f, 0.0f);
	}
	objects[0]->isPlayerControllable = true;
}

void FiveSquares::Draw()
{
	float color[] = { 0.5, 0.5f, 0.5f, 1.0f };
	context->ClearRenderTargetView(renderView, color);
	shaders->Draw(context);
	Matrix data;
	D3D11_MAPPED_SUBRESOURCE res = {};

	UpdateControllables();

	GameComponent* currentSquare = nullptr;

	for (auto object : objects)
		if (object->isPlayerControllable)
			currentSquare = object;

	if (input->IsKeyDown(Keys::W)) {
		if (input->IsKeyDown(Keys::A)) {
			currentSquare->translation.x += cos(135.0f / 360.0f * 6.28f) * 0.01;
			currentSquare->translation.y += sin(135.0f / 360.0f * 6.28f) * 0.01;
		}
		else if (input->IsKeyDown(Keys::D)) {
			currentSquare->translation.x += cos(45.0f / 360.0f * 6.28f) * 0.01;
			currentSquare->translation.y += sin(45.0f / 360.0f * 6.28f) * 0.01;
		}
		else
			currentSquare->translation.y += sin(90.0f / 360.0f * 6.28f) * 0.01;
	}
	else if (input->IsKeyDown(Keys::S)) {
		if (input->IsKeyDown(Keys::A)) {
			currentSquare->translation.x += cos(225.0f / 360.0f * 6.28f) * 0.01;
			currentSquare->translation.y += sin(225.0f / 360.0f * 6.28f) * 0.01;
		}
		else if (input->IsKeyDown(Keys::D)) {
			currentSquare->translation.x += cos(315.0f / 360.0f * 6.28f) * 0.01;
			currentSquare->translation.y += sin(315.0f / 360.0f * 6.28f) * 0.01;
		}
		else
			currentSquare->translation.y += sin(270.0f / 360.0f * 6.28f) * 0.01;
	}
	else if (input->IsKeyDown(Keys::A)) {
		currentSquare->translation.x += cos(180.0f / 360.0f * 6.28f) * 0.01;
	}
	else if (input->IsKeyDown(Keys::D)) {
		currentSquare->translation.x += cos(0.0f / 360.0f * 6.28f) * 0.01;
	}

	for (int i = 0; i < objects.size(); ++i) {

		data = Matrix::CreateScale(objects[i]->scale);
		data *= Matrix::CreateFromYawPitchRoll(objects[i]->rotation.ToEuler());
		data *= Matrix::CreateTranslation(objects[i]->translation);

		context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);

		data.Transpose();

		auto dataPtr = reinterpret_cast<float*>(res.pData);
		memcpy(dataPtr, &data, sizeof(data));

		objects[i]->Draw(context);

		context->Unmap(constantBuffer, 0);
	}
}

void FiveSquares::UpdateControllables()
{
	UINT keyNum = 0;
	if (input->IsKeyDown(Keys::D1)) {
		keyNum = UINT(Keys::D1);
	}
	else if (input->IsKeyDown(Keys::D2)) {
		keyNum = UINT(Keys::D2);
	}
	else if (input->IsKeyDown(Keys::D3)) {
		keyNum = UINT(Keys::D3);
	}
	else if (input->IsKeyDown(Keys::D4)) {
		keyNum = UINT(Keys::D4);
	}
	else if (input->IsKeyDown(Keys::D5)) {
		keyNum = UINT(Keys::D5);
	}

	if (keyNum) {
		for (auto object : objects)
			object->isPlayerControllable = false;
		objects[keyNum - 49]->isPlayerControllable = true;
	}
}

