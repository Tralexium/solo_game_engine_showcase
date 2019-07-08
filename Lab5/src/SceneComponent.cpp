#include "SceneComponent.h"
#include "Game.h"
#include "DuckGame.h"

void SceneComponent::changeSceneJson()
{
	//DuckGame* game = (DuckGame*)m_game;
	DuckGame* game = dynamic_cast<DuckGame*>(m_game);
	game->changeSceneJson(m_scenes.at(m_sceneId));
}