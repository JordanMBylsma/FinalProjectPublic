#pragma once
#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"


class SpellComponent : public Component
{
public :
	Vector2D pausedVelocity;
	SpellComponent(float range, float speed, int spellFacing) : range(range), speed(speed), spellFacing(spellFacing)
	{}
	~SpellComponent()
	{}

	void initialize() override
	{
		// 0 = up, 1 = upRight, 2 = right, 3= downRight, 4 = down, 5 = down left, 6 = left, 7 = up left
		movement = &entity->getComponent<MovementComponent>();
		switch (spellFacing)
		{
		case 0:
			movement->velocity = Vector2D(0.0f, -2.828f);
			break;
		case 1:
			movement->velocity = Vector2D(2.0f, -2.0f);
			break;
		case 2:
			movement->velocity = Vector2D(2.828f, 0.0f);
			break;
		case 3:
			movement->velocity = Vector2D(2.0f, 2.0f);
			break;
		case 4:
			movement->velocity = Vector2D(0.0f, 2.828f);
			break;
		case 5:
			movement->velocity = Vector2D(-2.0f, 2.0f);
			break;
		case 6:
			movement->velocity = Vector2D(-2.828f, 0.0f);
			break;
		case 7:
			movement->velocity = Vector2D(-2.0f, -2.0f);
			break;
		default:
			movement->velocity = Vector2D(0.0f, 0.0f);
			break;
		}

	}

	void updateLoop() override
	{
		if (movement->position.x > 800.0f or movement->position.y > 640.0f or movement->position.x + movement->width < 0.0f or movement->position.y + movement->height < 0.0f)
		{
			entity->destroy();
		}
	}


private:
	MovementComponent* movement;
	float range;
	float speed;
	int spellFacing;

};
