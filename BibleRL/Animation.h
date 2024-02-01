#pragma once

struct Animation
{
	int index;
	int frames;
	int speed;

	Animation() {}
	Animation(int mIndex, int mFrames, int mSpeed)
	{
		index = mIndex;
		frames = mFrames;
		speed = mSpeed;
	}

};