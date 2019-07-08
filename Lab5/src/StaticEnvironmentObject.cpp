#include "StaticEnvironmentObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"

StaticEnvironmentObject::StaticEnvironmentObject(Model *modelId, const glm::vec3& pos, const glm::quat& orient, const glm::vec3& scale)
{
	addComponent(new TransformComponent(pos, orient, scale));
	addComponent(new ModelComponent(modelId));
}

StaticEnvironmentObject::~StaticEnvironmentObject()
{
	// Delete anything that consumes memory
}

void StaticEnvironmentObject::OnUpdate(float dt)
{
}

void StaticEnvironmentObject::OnMessage(const std::string msg)
{
}