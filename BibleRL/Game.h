#pragma once

#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "Vector2D.h"


class ObjectController;
class ColliderComponent;

class Game 
{

public:
	Game();
	~Game();

	void initialize(const char* title, int xPos, int yPos, int width, int height, bool fullScreen);
	
	void eventLoop();
	void updateLoop();
	bool isRunning() { return running; }
	void renderLoop();
	void cleanLoop();
	void CastSpell(bool castIn, int facing);
	void LoadGame();
	bool MouseClick(Vector2D button, float width, float height, float scale);
	void CreateXP(Vector2D xpLocation);
	

	

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static ObjectController* objects;
	static std::vector<ColliderComponent*> colliders;
	bool running;
	enum groupingEntities : std::size_t
	{
		allPlayers,
		allOpposers,
		allPowerUps,
		allSpells,
		allLabels,
		allMenus,
		allUpgrades
	};

	static void AddTile(int id, int x, int y);

private:
	SDL_Window* window;
	int counter = 0;
};