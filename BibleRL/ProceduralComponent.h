#pragma once
#include "ECS.h"

class ProceduralComponent : public Component
{
public:
	int map[25][20];
	int id;
	int doors; //random from 1-4

	ProceduralComponent();
	~ProceduralComponent();
	ProceduralComponent(int mapId)
	{
		id = mapId;
	}

	void init() override
	{
	}


};