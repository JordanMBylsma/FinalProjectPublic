#pragma once
#include "Components.h"
#include "Vector2D.h"

class MovementComponent : public Component
{
public:
	bool paused = false;
	Vector2D position;
	Vector2D velocity;
	Vector2D circlePos;
	float height = 16.0f;
	float width = 16.0f;
	float scale = 1.0f;
	float speed = 1.0f;
	float originalSpeed = 1.0f;
	float radius=10.0f;
	float player = 0.0f;
	Vector2D normVelocity;

	MovementComponent()
	{
		position.Zero();
		circleRadius();
	}
	MovementComponent(float scaleIn)
	{
		position.Zero();
		scale = scaleIn;
		circleRadius();
	}
	MovementComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
		circleRadius();
	}
	MovementComponent(float x, float y, float scaleIn)
	{
		position.x = x;
		position.y = y;
		scale = scaleIn;
		circleRadius();
	}
	MovementComponent(float x, float y, float h, float w, float scaleIn)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = scaleIn;
		circleRadius();
	}
	MovementComponent(float x, float y, float h, float w, float scaleIn, float speedIn)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = scaleIn;
		originalSpeed = speedIn;
		speed = originalSpeed;
		circleRadius();
	}
	MovementComponent(float x, float y, float h, float w, float scaleIn, Vector2D velocityIn, float speedIn)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = scaleIn;
		velocity = velocityIn;
		originalSpeed = speedIn;
		speed = originalSpeed;
		circleRadius();
	}
	void initialize() override
	{
		velocity.Zero();
	}
	void updateLoop() override
	{
		if (paused == false)
		{
			position.x += velocity.x * speed;
			position.y += velocity.y * speed;
			circleRadius();
		}
	}
	void followPlayer(Vector2D playerPosition)
	{
		normVelocity.FindNormVector((playerPosition+Vector2D(16.0f, 24.0f))- position, 1);
		velocity = normVelocity;
	}
	void circleRadius()
	{
		circlePos.x = position.x + (scale * width) / 2.0f ;
		circlePos.y = (position.y + (scale * height) / 2.0f) + player;
		radius;
	}
};