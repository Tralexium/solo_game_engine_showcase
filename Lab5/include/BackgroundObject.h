#pragma once
#include "GameObject.h"

class BackgroundObject : public GameObject
{
public:
	BackgroundObject();
	~BackgroundObject();

	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
};