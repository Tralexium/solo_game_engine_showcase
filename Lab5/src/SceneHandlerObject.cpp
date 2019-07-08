#include "SceneHandlerObject.h"
#include "SceneComponent.h"

SceneHandlerObject::SceneHandlerObject(Game* gameId)
{
	addComponent(new SceneComponent(gameId));
}

SceneHandlerObject::~SceneHandlerObject()
{
}

void SceneHandlerObject::OnUpdate(float dt)
{
}

void SceneHandlerObject::OnMessage(const std::string msg)
{
	if (msg == "NextScene") { getComponent<SceneComponent>()->OnMessage(msg); }
	else if (msg == "PreviousScene") { getComponent<SceneComponent>()->OnMessage(msg); }
}
