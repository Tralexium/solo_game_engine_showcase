#pragma once

#include "GameObject.h"
#include "Model.h"

class SizedEnvironmentObject : public GameObject
{
public:
	SizedEnvironmentObject(Model *modelId, const glm::vec3& pos, const glm::quat& orient);
	~SizedEnvironmentObject();

	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
	float randomRange(float min, float max);
};
