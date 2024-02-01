#pragma once
#include "Components.h"

class UpgradeComponent : public Component
{
public:
	int upgradeType;
	UpgradeComponent(int upgradeTypeIn)
	{
		upgradeType = upgradeTypeIn;
	}
	~UpgradeComponent(){}
};