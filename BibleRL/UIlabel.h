#pragma once
#include "ECS.h"
#include "ObjectController.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UILabel : public Component
{
public:
	SDL_Rect position;
	UILabel(int xPosition, int yPosition, std::string text, std::string font, SDL_Color& color) :
		labelText(text), labelFont(font), labelColor(color)
	{
		position.x = xPosition;
		position.y = yPosition;
	}
	~UILabel()
	{
	}

	void SetLabelText(std::string text, std::string font)
	{
		SDL_Surface* surf = TTF_RenderText_Blended(Game::objects->GetFont(font), text.c_str(), labelColor);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}
	void drawLoop() override
	{
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}

private:
	
	std::string labelText;
	std::string labelFont;
	SDL_Color labelColor;
	SDL_Texture* labelTexture;
};