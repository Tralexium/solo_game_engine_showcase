#pragma once

#include "GameObject.h"
#include "Model.h"

class WigglyEnvironmentObject : public GameObject
{
public:
	WigglyEnvironmentObject(Model *modelId, const glm::vec3& pos, const glm::quat& orient);
	~WigglyEnvironmentObject();

	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
	float randomRange(float min, float max);

private:
	float initSize;
	float wiggleAngle;
};
