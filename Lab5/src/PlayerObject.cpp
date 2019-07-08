#include "PlayerObject.h"

PlayerObject::PlayerObject(Model* modelId, const glm::vec3& pos, const glm::quat& orient, const glm::vec3& scale) : m_cameraState(CameraViewState::firstPersonCamera)
{
	addComponent(new TransformComponent(pos, orient, scale));
	addComponent(new CameraComponent);
	addComponent(new ModelComponent(modelId));

	m_moveSpeed = 0.10;
}

PlayerObject::~PlayerObject()
{
	// Remove the components and other memory stuff
}

void PlayerObject::OnUpdate(float dt)
{
	switch (m_cameraState)
	{
		case CameraViewState::firstPersonCamera:
		{
			// get the transform component details
			TransformComponent* transform = getComponent<TransformComponent>();
			// first person view - just have the camera in place on the player
			// need inverse pos and orientation
			glm::quat inverseOrientation = inverse(transform->m_orientation);
			getComponent<CameraComponent>()->m_position = (transform->m_position + glm::vec3(0, 1.0f, -1.0f) * inverseOrientation);
			getComponent<CameraComponent>()->m_orientation = transform->m_orientation * m_mouseTravel;
			break;
		}

		case CameraViewState::thirdPersonCamera:
		{
			// get the transform component details
			TransformComponent* transform = getComponent<TransformComponent>();
			// third person view - stay at a set distance from the player
			glm::quat inverseOrientation = inverse(transform->m_orientation);
			getComponent<CameraComponent>()->m_position = (transform->m_position + glm::vec3(0, 5.0f, 8.0f) * inverseOrientation);
			getComponent<CameraComponent>()->m_orientation = transform->m_orientation * m_mouseTravel;
			break;
		}
	}
}

void PlayerObject::OnMessage(const std::string msg)
{
	if (msg == "switchCamera")
	{
		if(m_cameraState == CameraViewState::thirdPersonCamera)
			m_cameraState = CameraViewState::firstPersonCamera;
		else m_cameraState = CameraViewState::thirdPersonCamera;
	}
	else if (msg.compare(0, 12, "rotateCamera") == 0)
	{
		TransformComponent* transform = getComponent<TransformComponent>();
		float rotationValue;
		if (msg == "rotateCameraLeft") rotationValue = 0.05;
		else rotationValue = -0.05;
		// same rotation for both first and third person cameras
		transform->yaw(rotationValue);
	}
	else if (msg.compare(0, 10, "movePlayer") == 0)
	{
		// determine the correct translation vector
		glm::vec3 translationVector(0, 0, 0);
		if (msg == "movePlayerForward")translationVector.z = -m_moveSpeed;
		else if (msg == "movePlayerBackward")translationVector.z = m_moveSpeed;
		else if (msg == "movePlayerLeft")translationVector.x = -m_moveSpeed;
		else if (msg == "movePlayerRight")translationVector.x = m_moveSpeed;
		TransformComponent* transform = getComponent<TransformComponent>();
		transform->translate(translationVector * inverse(transform->m_orientation));
	}
}

void PlayerObject::MouseTravel(glm::vec2 travelDist)
{
	TransformComponent* transform = getComponent<TransformComponent>();

	transform->yaw(travelDist.x/500.0f);
	m_mouseTravel *= glm::angleAxis(travelDist.y/500.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}
