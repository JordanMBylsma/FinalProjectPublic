#include "Collision.h"
#include "ColliderComponent.h"

float distanceSQRD;
float pen;
Vector2D centerA;
Vector2D centerB;
Vector2D distance;
int result = 0;

int Collision::AABB(const SDL_Rect& rectA, const SDL_Rect& rectB)
{
	result = 0;
	if (
		rectA.x + rectA.w >= rectB.x &&
		rectB.x + rectB.w >= rectA.x &&
		rectA.y + rectA.h >= rectB.y &&
		rectB.y + rectB.h >= rectA.y
		)
	{
		result = 1;
	}
	return result;
	
}

int Collision::AABB(const ColliderComponent& colliderA, const ColliderComponent& colliderB)
{
	result = 0;
	return result;
}

int Collision::BoundingBallSpell(const SDL_Rect& rectA, const SDL_Rect& rectB, float spellRadius)
{
	result = 0;
	centerA = Vector2D(rectA.x + (rectA.w / 2.0f), rectA.y + (rectA.h / 2.0f));
	centerB = Vector2D(rectB.x + (rectB.w / 2.0f), rectB.y + (rectB.h / 2.0f));
	distance = centerB - centerA;
	//figure out distance
	distanceSQRD = (distance.x * distance.x) + (distance.y * distance.y);
	//check if coliding
	if (distanceSQRD <= spellRadius)
	{
		result = 1;
	}
	return result;
}
int Collision::Background(const SDL_Rect& rectA)
{
	result = 0;
	if (
		rectA.x <= 16 or
		rectA.x + rectA.w >= 784 or
		rectA.y <= 24 or
		rectA.y + rectA.h >= 600
		)
	{
		result = 1;
	}
	return result;
}
bool Collision::BoundingBallEnemy(const MovementComponent A, const MovementComponent B, float &pushout, Vector2D &directionNorm)
{
	Vector2D direction = Vector2D(B.circlePos.x - A.circlePos.x, B.circlePos.y - A.circlePos.y);
	float distanceSQRD = pow(direction.x, 2) + pow(direction.y, 2);
	if (distanceSQRD < (A.radius + B.radius) * (A.radius + B.radius))
	{
		pushout = (float)sqrt(distanceSQRD) - A.radius - B.radius;
		
		directionNorm.FindNormVector(direction, 1.0f);
		return true;
	}
	else
	{
		return false;
	}
}

