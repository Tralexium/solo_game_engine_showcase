#pragma once
#include "GameObject.h"
#include "Game.h"

class SceneHandlerObject : public GameObject
{
public:
	SceneHandlerObject(Game* gameId);
	~SceneHandlerObject();

	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
};