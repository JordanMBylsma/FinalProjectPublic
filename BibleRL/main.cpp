#include "Game.h"

Game* game = nullptr;

int main(int argc, char* argv[]) 
{
	const int targetFramesPerSecond = 60;
	const int delayBetweenFrames = 1000 / targetFramesPerSecond;
	const int screenWidth = 800;
	const int screenHeight = 672;

	game = new Game();
	game->initialize("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, false);

	while (game->isRunning()) {

		Uint32 frameStart = SDL_GetTicks();

		game->eventLoop();
		game->updateLoop();
		game->renderLoop();

		int frameTime = SDL_GetTicks() - frameStart;
		if (delayBetweenFrames > frameTime)
		{
			SDL_Delay(delayBetweenFrames - frameTime);
		}
	}
	game->cleanLoop();
	delete game;
	return 0;
}