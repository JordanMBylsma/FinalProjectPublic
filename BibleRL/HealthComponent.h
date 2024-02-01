#pragma once
#include "Components.h"

class HealthComponent : public Component
{	
public:
	float health;
	HealthComponent() 
	{
		health = 100;
	}
	~HealthComponent()
	{}

	HealthComponent(float healthIn)
	{
		health = healthIn;
	}
	bool IsDead()
	{
		if (health <= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	



};
