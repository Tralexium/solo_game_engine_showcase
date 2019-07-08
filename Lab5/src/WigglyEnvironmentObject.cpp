#include "WigglyEnvironmentObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include <random>
#include <math.h>

WigglyEnvironmentObject::WigglyEnvironmentObject(Model *modelId, const glm::vec3& pos, const glm::quat& orient)
{
	initSize = randomRange(1.0f, 1.5f);
	glm::vec3 scale = glm::vec3(initSize);

	addComponent(new TransformComponent(pos, orient, scale));
	addComponent(new ModelComponent(modelId));
}

WigglyEnvironmentObject::~WigglyEnvironmentObject()
{
}

void WigglyEnvironmentObject::OnUpdate(float dt)
{
	wiggleAngle += dt;
	float spd = 100.0f;
	float wiggleX = initSize + (std::sin(wiggleAngle / spd*1.3f) * 0.2f);
	float wiggleY = initSize + (std::sin((wiggleAngle + 45.0f) / spd) * 0.05f);
	float wiggleZ = initSize + (std::sin((wiggleAngle + 90.0f) / spd) * 0.2f);
	glm::vec3 wiggle = glm::vec3(wiggleX, wiggleY, wiggleZ);

	TransformComponent* transform = getComponent<TransformComponent>();
	transform->setScale(wiggle);
}

void WigglyEnvironmentObject::OnMessage(const std::string msg)
{
}

float WigglyEnvironmentObject::randomRange(float min, float max) {
	std::random_device rd; // Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> d(min, max); // Set a range for the final random value
	return d(gen);
}