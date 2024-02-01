#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include "SDL_ttf.h"

class ObjectController
{
public:
	ObjectController(Manager* man);
	~ObjectController();

	//gameobjects

	void CreateSpell(Vector2D pos, float range, float speed, std::string id, int spellFacing);
	void CreateUpgrade(Vector2D pos, int id);
	void CreateEnemy(Vector2D pos, Vector2D vel, float speed, std::string id);
	void CreateXP(Vector2D pos, std::string id);

	//texture management
	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

	void AddFont(std::string id, std::string path, float fontSize);
	TTF_Font* GetFont(std::string id);

private:

	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
};
