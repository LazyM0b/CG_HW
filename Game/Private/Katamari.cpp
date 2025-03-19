#include "Katamari.h"

Katamari::Katamari(HINSTANCE hinst, LPCTSTR hwindow) : Game(hinst, hwindow) {};

void Katamari::Initialize(UINT objCnt) {
	Game::Initialize();

	std::vector<Vector4> colors;

	Vector3 color((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f);
	colors.push_back(Vector4(color.x, color.y, color.z, 1.0f));
	colors.push_back(Vector4(color.z, color.y, color.x, 1.0f));

	color = Vector3((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f);
	colors.push_back(Vector4(color.x, color.y, color.z, 1.0f));
	colors.push_back(Vector4(color.z, color.y, color.x, 1.0f));

	color = Vector3((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f);
	colors.push_back(Vector4(color.x, color.y, color.z, 1.0f));
	colors.push_back(Vector4(color.z, color.y, color.x, 1.0f));

	color = Vector3((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f);
	colors.push_back(Vector4(color.x, color.y, color.z, 1.0f));
	colors.push_back(Vector4(color.z, color.y, color.x, 1.0f));

	objectTypes.push_back(Cube);
	objects.push_back(new GameComponent());
	objects[0]->Initialize(device, objectTypes[0], colors, LOD);

	objectTypes.push_back(Sphere);
	objects.push_back(new GameComponent());
	objects[1]->Initialize(device, objectTypes[1], colors, LOD);

	std::vector<std::string> modelPaths;
	std::vector<const wchar_t*> texturePaths;

	modelPaths.push_back("Game\\Objects\\banana.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\banana.png");
	modelPaths.push_back("Game\\Objects\\blackberry.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\blackberry.png");
	modelPaths.push_back("Game\\Objects\\cherry.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\cherry.png");
	modelPaths.push_back("Game\\Objects\\coconut.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\coconut.png");
	modelPaths.push_back("Game\\Objects\\coconut_green.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\coconut_green.png");
	modelPaths.push_back("Game\\Objects\\grape.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\grape.png");
	modelPaths.push_back("Game\\Objects\\lemon.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\lemon.png");
	modelPaths.push_back("Game\\Objects\\lime.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\lime.png");
	modelPaths.push_back("Game\\Objects\\mango.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\mango.png");
	modelPaths.push_back("Game\\Objects\\orange.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\orange.png");
	modelPaths.push_back("Game\\Objects\\papaya.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\papaya.png");
	modelPaths.push_back("Game\\Objects\\pear.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\pear.png");
	modelPaths.push_back("Game\\Objects\\pineapple.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\pineapple.png");
	modelPaths.push_back("Game\\Objects\\strawberry.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\strawberry.png");
	modelPaths.push_back("Game\\Objects\\watermelon.fbx");
	texturePaths.push_back((const wchar_t*)L"Game\\Objects\\watermelon.png");

	for (int i = 0; i < modelPaths.size(); ++i) {
		objectTypes.push_back(Mesh);
		objects.push_back(new GameComponent());
		objects[i + 2]->Initialize(device, objectTypes[i + 2], modelPaths[i], texturePaths[i]);
	}

	for (int i = objects.size(); i < objCnt; ++i) {
		objectTypes.push_back(Mesh);
		objects.push_back(new GameComponent(device, *objects[rand() % modelPaths.size() + 2]));
	}

	for (auto object : objects)
		object->collisionEnabled = true;
	objects[0]->collisionEnabled = false;

	objects[0]->isMovable = false;
	for (int i = 1; i < objCnt; ++i)
		objects[i]->isMovable = true;

	ResetGame();
}

void Katamari::Draw()
{
	Game::Draw();

	if (camManager->objectToTrack != -1) {
		camManager->UpdatePos(input, objects[camManager->objectToTrack]->translation, clientWidth, clientHeight, &objects[camManager->objectToTrack]->impulse);
		controller->UpdatePos(input, objects[camManager->objectToTrack]);
	}
	else
		camManager->UpdatePos(input, camManager->objectPos, clientWidth, clientHeight);
	for (int i = 2; i < objects.size(); ++i) {
		if (objects[i]->collisionEnabled && objects[1]->sphereCollider.Intersects(objects[i]->sphereCollider) && objects[1]->sphereCollider.Radius > objects[i]->sphereCollider.Radius) {
			++objectsInside; 
			objects[i]->parent = objects[1];
			objects[i]->collisionEnabled = false;
			objects[i]->distanceToParent = objects[1]->translation - Vector3(objects[i]->translation.x, objects[i]->scale.x / 2, objects[i]->translation.z);
			//pos.Normalize();
			/*pos.x *= cos(objects[1]->rotation.ToEuler().y);
			pos.z *= sin(objects[1]->rotation.ToEuler().y);*/
			objects[1]->sphereCollider.Radius += objects[i]->sphereCollider.Radius / (objectsInside * 2.71828);
			camManager->distance += objectsInside * objects[i]->scale.x / 100;
		}
		else if (!objects[i]->collisionEnabled) {
			objects[i]->translation = Vector3(cos(objects[i]->parent->rotation.ToEuler().y), -sin(objects[i]->parent->rotation.ToEuler().y), sin(objects[i]->parent->rotation.ToEuler().y)) * objects[i]->distanceToParent;
		}
	}

	for (auto object : objects)
		object->Draw(context, camManager);
}

void Katamari::ResetGame()
{
	objects[0]->scale = Vector3(100000.0f, 0.1f, 100000.0f);
	objects[1]->scale = Vector3(100.0f, 100.0f, 100.0f);
	objects[1]->translation = Vector3(0.0f, 100.0f, 0.0f);
	objects[1]->isPlayerControllable = true;
	objects[1]->speed = 1.0f;
	objects[1]->speedMax = 100.0f;

	objects[1]->sphereCollider.Radius = objects[1]->scale.x;
	objects[1]->sphereCollider.Center = objects[1]->translation;

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> random1(0, 30000);

	for (int i = 2; i < objects.size(); ++i) {
		float scale = rand() % 1000 + 100.0f;
		objects[i]->scale = Vector3(scale, scale, scale);
		objects[i]->translation = Vector3((int)random1(rng) - 15000, 0.0f, (int)random1(rng) - 15000);
		objects[i]->sphereCollider.Radius = objects[i]->scale.x / 3;
		objects[i]->sphereCollider.Center = objects[i]->translation;
	}

	float aspectRatio = (float) clientWidth / (float) clientHeight;
	camManager->Initialize(aspectRatio);
	camManager->objectToTrack = 1;
	camManager->isStationary = 0;

	camManager->SetCameraView(objects[1]->translation, Vector3(0.0f, objects[1]->scale.x * 2.0f, objects[1]->scale.x * 7.0f));
}
