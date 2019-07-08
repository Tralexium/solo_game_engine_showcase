#pragma once
#include "Game.h"
#include "SceneHandlerObject.h"
#include "FrameBufferObject.h"
#include "VBOQuad.h"

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
	FrameBufferObject* m_frameBuffer;
	VBOQuad* m_vboQuad;
};
