#include "RotatingEnvironmentObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include <random>

RotatingEnvironmentObject::RotatingEnvironmentObject(Model *modelId, const glm::vec3& pos, const glm::quat& orient, float rotSpd)
{
	addComponent(new TransformComponent(pos, orient));
	addComponent(new ModelComponent(modelId));

	rotationSpeed = rotSpd; // Set the rotation speed
}

RotatingEnvironmentObject::~RotatingEnvironmentObject()
{
}

void RotatingEnvironmentObject::OnUpdate(float dt)
{
	TransformComponent* orientation = getComponent<TransformComponent>();
	orientation->rotate(dt*rotationSpeed, 0.0f, 1.0f, 0.0f);
}

void RotatingEnvironmentObject::OnMessage(const std::string msg)
{
}

void RotatingEnvironmentObject::SetRotSpeed(float rotSpd)
{
	rotationSpeed = rotSpd; // Set the rotation speed
}