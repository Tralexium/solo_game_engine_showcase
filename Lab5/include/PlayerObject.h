#pragma once
#include "GameObject.h"
#include "Model.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ModelComponent.h"

class PlayerObject : public GameObject
{
	enum class CameraViewState { firstPersonCamera, thirdPersonCamera };
public:
	PlayerObject(Model* modelId, const glm::vec3& pos, const glm::quat& orient, const glm::vec3& scale);
	~PlayerObject();
	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
	void MouseTravel(glm::vec2 travelDist);

private:
	CameraViewState m_cameraState;
	float m_moveSpeed;
	glm::quat m_mouseTravel = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
};