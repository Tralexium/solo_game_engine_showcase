#pragma once
#include "GameObject.h"
#include "Model.h"

class StaticEnvironmentObject : public GameObject
{
public:
	StaticEnvironmentObject(Model *modelId, const glm::vec3& pos, const glm::quat& orient, const glm::vec3& scale);
	~StaticEnvironmentObject();

	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
};
