#pragma once
#include "IEngineCore.h"
#include "InputHandler.h"
#include "Scene.h"

class Game
{
public:
	virtual ~Game() {}
	IEngineCore* m_engineInterfacePtr;
	InputHandler* m_inputHandler;
	Scene* m_currentScene;

	virtual void update(float dt) = 0;
	virtual void render() = 0;
};
