#pragma once
#include "Game.h"
#include "ECS.h"
#include "Components.h"


class MouseController : public Component
{
public:
	Vector2D mousePosition;
	int mousePosX;
	int mousePosY;
	bool mousePress = false;
	MouseController() {}
	~MouseController() {}

	void updateLoop() override
	{
		if (Game::event.type == SDL_MOUSEBUTTONDOWN && Game::event.button.button == SDL_BUTTON_LEFT)
		{
			SDL_GetMouseState(&mousePosX, &mousePosY);
			mousePress = true;
			mousePosition = Vector2D(mousePosX, mousePosY);
		}
		else
		{
			mousePosition = Vector2D(0, 0);
		}
		
	}
};
