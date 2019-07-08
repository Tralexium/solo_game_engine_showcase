#pragma once
#include "IEngineCore.h"
#include "GameObject.h"
#include "BackgroundObject.h"
#include "CameraComponent.h"
#include "ModelManager.h"
#include "json/json.h"

class Scene
{
public:
	Scene();
	~Scene();

	//void loadLevel(std::string levelFile); // LEGACY LEVEL LOADING
	bool loadJsonLevel(std::string levelJsonFile);
	void updateScene(float dt);
	void renderScene(IEngineCore* engine, CameraComponent* cam);
	GameObject* getObjectAt(int index);
	std::vector<GameObject*> getInputObjects();

private:
	GameObject* m_playerBackground = new BackgroundObject(); // TODO: Make background and sceneHandler their own objects!
	ModelManager m_modelManager;

	std::vector<GameObject*> v_sceneObjects; // Changed it to a pointer to access the individual object
	std::vector<GameObject*> v_inputObjects; // Objects that require input from the user
};