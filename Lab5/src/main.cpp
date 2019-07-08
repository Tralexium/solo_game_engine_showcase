#define USE_GLFW 1
#ifdef USE_GLFW
	#include "GLFW_EngineCore.h"
#endif

#include "Game.h"
#include "DuckGame.h"

int main(int argc, char* argv[])
{
	IEngineCore* engineCore;

#ifdef USE_GLFW
	engineCore = new GLFW_EngineCore;
#endif

	if (!engineCore->initWindow(800, 600, "TransformAndData"))
		return -1;

	Game* myDuckGame = new DuckGame();

	engineCore->runEngine(myDuckGame);

	return 0;
}
