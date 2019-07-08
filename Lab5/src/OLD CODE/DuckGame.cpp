#include "DuckGame.h"
#include "TransformComponent.h"

DuckGame::DuckGame()
{
	m_engineInterfacePtr = nullptr;
	m_currentScene = new Scene();
	m_sceneHandler = new SceneHandlerObject(this);

	m_currentScene->loadJsonLevel("assets/levels/level1.json");
	v_inputObjects = m_currentScene->getInputObjects();
	v_inputObjects.push_back(m_sceneHandler);

	m_inputHandler = new InputHandler(v_inputObjects);
}


void DuckGame::update(float dt)
{
	m_currentScene->updateScene(dt);
}

void DuckGame::render()
{
	// update the camera
	GameObject* player = v_inputObjects[0];
	//GLuint& shaderProgram = player->getComponent<ShaderComponent>()->shaderProgram; // TODO: pass uniforms to shaderComponent class instead of here
	//->setCamera(player->getComponent<CameraComponent>(), shaderProgram);

	// Request the scene to render
	m_currentScene->renderScene(m_engineInterfacePtr, player->getComponent<CameraComponent>());

	// Text drawing
	/*
	TransformComponent* playerTrans = player->getComponent<TransformComponent>();
	glm::vec3 pPos = playerTrans->position();
	m_engineInterfacePtr->renderText("X: " + to_string(pPos.x) + " Y: " + to_string(pPos.y) + " Z: " + to_string(pPos.z),
									0.005f, 0.97f, 0.3f, glm::vec3(0.0f, 1.0f, 0.5f));
	*/
}

void DuckGame::changeSceneJson(std::string jsonFile)
{
	delete m_currentScene;

	m_currentScene = new Scene();
	m_currentScene->loadJsonLevel(jsonFile);
	m_inputHandler->clearObjects();
	v_inputObjects.clear();

	v_inputObjects = m_currentScene->getInputObjects();
	v_inputObjects.push_back(m_sceneHandler);

	for (const auto& obj : v_inputObjects) // Iterate through all objects and pass them to InputHandler
	{
		m_inputHandler->addObject(*obj);
	}
}
