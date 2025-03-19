#include "PlayerController.h"
# define M_PI           3.14159265358979323846

void PlayerController::UpdatePos(InputDevice* input, GameComponent* object)
{
	Vector3 tmpVelocity;
	float tmpSpeed = object->speed;
	if (input->IsKeyDown(Keys::W)) {
		tmpVelocity += object->impulse * object->speed * -1;
	}
	if (input->IsKeyDown(Keys::S)) {
		tmpVelocity += object->impulse * object->speed;
	}
	if (input->IsKeyDown(Keys::A)) {
		tmpVelocity += Vector3(cos(asin(1.0f) + asin(object->impulse.z)), 0.0f, sin(asin(1.0f) + acos(object->impulse.x)));
	}
	if (input->IsKeyDown(Keys::D)) {
		tmpVelocity += Vector3(cos(asin(-1.0f) + asin(object->impulse.z)), 0.0f, sin(asin(-1.0f) + acos(object->impulse.x)));
	}
	if (input->IsKeyDown(Keys::Space) && object->translation.y == object->scale.y) {
		tmpVelocity += Vector3(0.0f, 20.0f, 0.0f);
	}
	if (input->IsKeyDown(Keys::LeftShift)) {
		tmpSpeed =  object->speed * 2;
	}
	if (abs(Vector3::Distance(object->velocity + tmpVelocity * tmpSpeed, Vector3::Zero)) < object->speedMax)
		object->velocity += tmpVelocity * object->speed;
	else
		std::cout << "dfl;asd" << std::endl;
	object->translation += object->velocity;

	float tmpRotation = 0.0f;
	if (object->velocity.x * object->velocity.z > 0 && object->velocity.z > 0)
		tmpRotation = object->velocity.z / 20.0f * M_PI / 2.0f;
	else if (object->velocity.x * object->velocity.z > 0 && object->velocity.z < 0)
		tmpRotation = object->velocity.z / 20.0f * -M_PI / 2.0f + M_PI;
	else if (object->velocity.z > 0)
		tmpRotation = M_PI - object->velocity.z / 20.0f * M_PI / 2.0f;
	else if (object->velocity.z < 0)
		tmpRotation = 2 * M_PI + object->velocity.z / 20.0f * M_PI / 2.0f;

	if (tmpRotation != 0.0f)
		object->rotation = Quaternion::CreateFromYawPitchRoll(-tmpRotation, 0.0f,
		object->rotation.ToEuler().z - Vector3::Distance(Vector3(object->velocity.x, 0.0f, object->velocity.z), Vector3::Zero) / object->speedMax);

	if (object->translation.y > object->scale.y && object->velocity.y < 10.0f) {
		object->velocity.y -= 1.0f;
	}
	else if (object->translation.y < object->scale.y){
		object->velocity.y = 0.0f;
		object->translation.y = object->scale.y;
	}
	object->velocity -= object->velocity / 20.0f;
}
