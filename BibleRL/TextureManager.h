#pragma once
#include "Game.h"

class TextureManager {

public:
	static SDL_Texture* LoadTexture(const char* fileName);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
	static void DrawRotate(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, float angle, SDL_Point *center);
};