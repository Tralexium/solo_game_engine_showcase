#pragma once
#include "Component.h"
#include <vector>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

class Game;

class SceneComponent : public Component
{
public:
	void OnUpdate(float dt) override {}
	void OnMessage(const std::string m) override 
	{
		if (m == "NextScene") { m_sceneId++; idRangeCheck(); changeSceneJson(); }
		else if (m == "PreviousScene") { m_sceneId--; idRangeCheck(); changeSceneJson(); }
	}

	SceneComponent(Game* game) : m_game(game) 
	{
		std::string path = "assets/levels/";
		for (const auto & entry : fs::directory_iterator(path))
			m_scenes.push_back(entry.path().string());
	}

	std::string getScene()
	{
		return m_scenes.at(m_sceneId);
	}

	void idRangeCheck() { // Make sure we don't go out of scope
		if (m_sceneId < 0) { m_sceneId = m_scenes.size() - 1; }
		else if (m_sceneId > m_scenes.size()-1) { m_sceneId = 0; }
	}

	void changeSceneJson();

private:
	Game* m_game;

	int m_sceneId;
	std::vector<std::string> m_scenes;
};