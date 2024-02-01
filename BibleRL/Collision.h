#pragma once
#include <SDL.h>
#include "Vector2D.h"
#include "Components.h"

class ColliderComponent;

class Collision
{
public:
	MovementComponent* movement;
	static int AABB(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static int AABB(const ColliderComponent& colliderA, const ColliderComponent& colliderB);
	static int BoundingBallSpell(const SDL_Rect& rectA, const SDL_Rect& rectB, float spellRadius);
	static int Background(const SDL_Rect& rectA);
	static bool BoundingBallEnemy(const MovementComponent A, const MovementComponent B, float& pushout, Vector2D& directionNorm);
};
