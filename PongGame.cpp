#include "PongGame.h"

PongGame::PongGame(HINSTANCE hinst, LPCTSTR hwindow): Game(hinst, hwindow) {}

void PongGame::Initialize(UINT objCnt)
{
	Game::Initialize();
	for (int i = 0; i < objCnt - 1; ++i)
		objectTypes.push_back(Square);
	objectTypes.push_back(Circle);

	for (int i = objCnt - 3; i < objCnt; ++i) {
		objects[i]->isMovable = true;
	}
	objects[objCnt - 3]->isPlayerControllable = true;
	
	ResetGame();
}

void PongGame::Draw() {
	Game::Draw();

	GameComponent* circle = objects[objects.size() - 1];

	if (input->IsKeyDown(Keys::Space))
		ResetGame();

	for (int i = 0; i < objects.size(); ++i) {

		//platform speed calculation
		if (objects[i]->isMovable && objects[i]->type == Square) {
			if (abs(objects[i]->translation.y) + objects[i]->scale.x < 1.0f) {

				float displacement = 0.0f;
				if ((objects[i]->isPlayerControllable && input->IsKeyDown(Keys::W)) || (!objects[i]->isPlayerControllable && circle->translation.y > objects[i]->translation.y)) {

					objects[i]->rotation = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 0.0f, 1.0f), asin(1.0f));
					displacement += objects[i]->rotation.ToEuler().z * objects[i]->speed;
				}
				else if ((objects[i]->isPlayerControllable && input->IsKeyDown(Keys::S)) || (!objects[i]->isPlayerControllable && circle->translation.y < objects[i]->translation.y)) {

					objects[i]->rotation = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 0.0f, 1.0f), asin(-1.0f));
					displacement += objects[i]->rotation.ToEuler().z * objects[i]->speed;
				}
				if (abs(objects[i]->translation.y + displacement) + objects[i]->scale.x < 1.0f)
					objects[i]->translation.y += displacement;
			}
		}

		//circle speed calculation
		if (objects[i]->type == Circle) {

			if (circle->translation.x + circle->scale.x > 1) {
				++score1;
				ShowScores();
				ResetGame();
			}
			else if (circle->translation.x - circle->scale.x < -1) {
				++score2;
				ShowScores();
				ResetGame();
			}
			else if (abs(circle->translation.y) + circle->scale.y >= 1) {
				circle->rotation = Quaternion::CreateFromYawPitchRoll( 2 * Vector3(1.0f, 0.0f, 0.0f) * Vector3(XMVector3Dot(circle->rotation.ToEuler(), Vector3(1.0f, 0.0f, 0.0f))) - circle->rotation.ToEuler());
				circle->impulse = Vector3(cos(circle->rotation.ToEuler().z) * circle->speed, sin(circle->rotation.ToEuler().z) * circle->speed, 0.0f);
			}

			for (int j = 0; j < i; ++j) {
				if (objects[j]->collisionEnabled && circle->boxCollider.Intersects(objects[j]->boxCollider)) {
					float circleAngle = atan2(sin(circle->rotation.ToEuler().z), -cos(circle->rotation.ToEuler().z));

					if (objects[j]->translation.y - circle->translation.y < 0)
						circleAngle += (asin(1.0f) - circleAngle) * abs(objects[j]->translation.y - circle->translation.y) / objects[j]->scale.x;
					else
						circleAngle += (asin(-1.0f) - circleAngle) * abs(objects[j]->translation.y - circle->translation.y) / objects[j]->scale.x;

					if (sin(objects[j]->rotation.ToEuler().z) * sin(circle->rotation.ToEuler().z) > 0)
 						circle->speed *= 1 + sin(objects[j]->rotation.ToEuler().z) / 5;
					else 
						circle->speed /= 1 + sin(objects[j]->rotation.ToEuler().z) / 5;

					circle->rotation = Quaternion::CreateFromAxisAngle( Vector3(0.0f, 0.0f, 1.0f), circleAngle);
					circle->impulse = Vector3(cos(circle->rotation.ToEuler().z) * circle->speed, sin(circle->rotation.ToEuler().z) * circle->speed, 0.0f);
					circle->translation += circle->impulse * 2;
				}
			}
			circle->translation += circle->impulse;
		}

	}
	for (auto object : objects)
		object->Draw(context, camManager);
}

void PongGame::ShowScores()
{
	std::cout << "Player 1 score: " << score1 << ", Player 2 score: " << score2 << std::endl;
}

//set initial translations and rotations
void PongGame::ResetGame() 
{
	float marksCnt = objects.size() - 3;

	for (int i = 0; i < objects.size(); ++i) {

		if (i < marksCnt) {
			objects[i]->scale = Vector3(0.03f, 0.03f, 0.0f);
			objects[i]->translation = Vector3(0.0f, (i - marksCnt / 2.0f) / (marksCnt / 2.0f) + 1.0f / marksCnt, 0.0f);
		}
		else {
			if (i == objects.size() - 1) {
				objects[i]->scale = Vector3(0.02f, 0.02f, 0.0f);
				objects[i]->translation = Vector3::Zero;
				float angle = std::rand() % 90 / 360.0f * 6.28f - 45.0f + 3.14f * float (std::rand() % 2);
				objects[i]->rotation = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 0.0f, 1.0f), angle);
				objects[i]->impulse = Vector3(cos(objects[i]->rotation.ToEuler().z) * 0.02, sin(objects[i]->rotation.ToEuler().z) * 0.02, 0.0f);
				objects[i]->speed = 0.02;
			}
			else {
				objects[i]->scale = Vector3(0.2f, 0.03f, 0.0f);
				objects[i]->rotation = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 0.0f, 1.0f), asin(1.0f));
				objects[i]->translation = Vector3(0.95 * pow(-1, i + 1), 0.0f, 0.0f);
				objects[i]->speed = 0.01;
			}

			objects[i]->collisionEnabled = true;
			objects[i]->boxCollider.Extents = Vector3(objects[i]->scale.y, objects[i]->scale.x, objects[i]->scale.z);
			objects[i]->boxCollider.Center = objects[i]->translation;
		}
	}
}
