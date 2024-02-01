#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture) 
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}
void TextureManager::DrawRotate(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, float angle, SDL_Point *center)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, center, SDL_FLIP_NONE);
}
