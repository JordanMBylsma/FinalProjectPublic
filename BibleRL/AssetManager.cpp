#include "ObjectController.h"
#include "Components.h"

ObjectController::ObjectController(Manager* man) : manager(man)
{}

ObjectController::~ObjectController()
{}

void ObjectController::CreateSpell(Vector2D pos, float range, float speed, std::string id, int spellFacing)
{

	auto& spell(manager->addEntity());
	spell.addComponent<MovementComponent>(pos.x, pos.y, 32.0f, 40.0f, 1.0f, speed);
	spell.addComponent<SpriteComponent>(id, true, spellFacing);
	spell.addComponent<SpellComponent>(range, speed, spellFacing);
	spell.addComponent<ColliderComponent>("spell");
	spell.addGroup(Game::allSpells);

}
void ObjectController::CreateUpgrade(Vector2D pos, int id)
{
	std::string spriteID;
	switch (id)
	{
	case 0:
		spriteID = "aoe";
		break;
	case 1:
		spriteID = "casting";
		break;
	case 2:
		spriteID = "damage";
		break;
	case 3:
		spriteID = "heal";
		break;
	case 4:
		spriteID = "health";
		break;
	case 5:
		spriteID = "pathing";
		break;
	case 6:
		spriteID = "shield";
		break;
	}
	auto& upgrade(manager->addEntity());
	upgrade.addComponent<MovementComponent>(pos.x, pos.y, 128, 128, 1.0f);
	upgrade.addComponent<SpriteComponent>(spriteID, false);
	upgrade.addComponent<UpgradeComponent>(id);
	upgrade.addGroup(Game::allUpgrades);
}

void ObjectController::CreateEnemy(Vector2D pos, Vector2D vel, float speed, std::string id)
{
	auto& enemy(manager->addEntity());
	enemy.addComponent<MovementComponent>(pos.x, pos.y, 32.0f, 32.0f, 1, vel, speed);
	enemy.addComponent<SpriteComponent>(id, false);
	enemy.addComponent<ColliderComponent>("opposer");
	enemy.addGroup(Game::allOpposers);
	enemy.addComponent<HealthComponent>();
}

void ObjectController::CreateXP(Vector2D pos, std::string id)
{
	auto& xp(manager->addEntity());
	xp.addComponent<MovementComponent>(pos.x, pos.y, 8.0f, 8.0f, 1.0f);
	xp.addComponent<SpriteComponent>(id, false);
	xp.addComponent<ColliderComponent>("powerUp");
	xp.addGroup(Game::allPowerUps);
}

void ObjectController::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* ObjectController::GetTexture(std::string id)
{
	return textures[id];
}

void ObjectController::AddFont(std::string id, std::string path, float fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}
TTF_Font* ObjectController::GetFont(std::string id)
{
	return fonts[id];
}