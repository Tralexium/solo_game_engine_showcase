#pragma once

#include "GameObject.h"
#include "Model.h"

class RotatingEnvironmentObject : public GameObject
{
public:
	RotatingEnvironmentObject(Model *modelId, const glm::vec3& pos, const glm::quat& orient, float rotSpd);
	~RotatingEnvironmentObject();

	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
	void SetRotSpeed(float rotSpd);

private:
	float rotationSpeed;
};