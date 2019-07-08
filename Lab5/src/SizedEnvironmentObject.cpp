#include "SizedEnvironmentObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include <random>

SizedEnvironmentObject::SizedEnvironmentObject(Model *modelId, const glm::vec3& pos, const glm::quat& orient) 
{
	float randVal = randomRange(1.0f, 2.0f);
	glm::vec3 scale = glm::vec3(randVal);

	addComponent(new TransformComponent(pos, orient, scale));
	addComponent(new ModelComponent(modelId));
}

SizedEnvironmentObject::~SizedEnvironmentObject()
{
}

void SizedEnvironmentObject::OnUpdate(float dt)
{
}

void SizedEnvironmentObject::OnMessage(const std::string msg)
{
}

float SizedEnvironmentObject::randomRange(float min, float max) {
	std::random_device rd; // Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> d(min, max); // Set a range for the final random value
	return d(gen);
}