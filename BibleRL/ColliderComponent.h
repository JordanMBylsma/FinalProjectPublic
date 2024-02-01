#pragma once
#include "Components.h"
#include "SDL.h"
#include <string>


class ColliderComponent : public Component
{ 
public:
	SDL_Rect collider;
	std::string tag;
	MovementComponent* movement;

	ColliderComponent(std::string tagIN) 
	{
		tag = tagIN;
	}
	void initialize() override
	{
		if (!entity->hasComponent<MovementComponent>())
		{
			entity->addComponent<MovementComponent>();
		}
		movement = &entity->getComponent<MovementComponent>(); 

		Game::colliders.push_back(this);
	}
	void updateLoop() override
	{
		collider.x = movement->position.x;
		collider.y = movement->position.y;
		collider.w = movement->width * movement->scale;
		collider.h = movement->height * movement->scale;
	}
};