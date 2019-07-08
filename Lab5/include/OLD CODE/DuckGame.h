#pragma once
#include "Game.h"
#include "SceneHandlerObject.h"

class DuckGame : public Game
{
public:
	DuckGame();
	~DuckGame() = default;
	void update(float dt) override;
	void render() override;

	void changeSceneJson(std::string jsonFile);

private:	
	vector<GameObject*> v_inputObjects;
	GameObject* m_sceneHandler;
};
