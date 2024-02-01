#pragma once
#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include "ObjectController.h"

class SpriteComponent : public Component
{
private:
	MovementComponent *movement;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames;
	int speed = 100;
	float spellAngle;

public:
	int animationIndex = 0;
	std::map<const char*, Animation> animations;

	SpriteComponent() = default;
	
	SpriteComponent(std::string id)
	{
		setText(id);
	}
	SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;
		if (id == "player")
		{
			//moving stances
			Animation down = Animation(1, 6, 180);
			Animation downLeft = Animation(4, 6, 180);
			Animation left = Animation(7, 6, 180);
			Animation upLeft = Animation(10, 6, 180);
			Animation up = Animation(13, 6, 180);
			Animation downRight = Animation(16, 6, 180);
			Animation right = Animation(19, 6, 180);
			Animation upRight = Animation(22, 6, 180);
			animations.emplace("Down", down);
			animations.emplace("Down Left", downLeft);
			animations.emplace("Left", left);
			animations.emplace("Up Left", upLeft);
			animations.emplace("Up", up);
			animations.emplace("Down Right", downRight);
			animations.emplace("Right", right);
			animations.emplace("Up Right", upRight);
			//idle stances
			Animation downIdle = Animation(0, 6, 180);
			Animation downLeftIdle = Animation(3, 6, 180);
			Animation leftIdle = Animation(6, 6, 180);
			Animation upLeftIdle = Animation(9, 6, 180);
			Animation upIdle = Animation(12, 6, 180);
			Animation downRightIdle = Animation(15, 6, 180);
			Animation rightIdle = Animation(18, 6, 180);
			Animation upRightIdle = Animation(21, 6, 180);
			animations.emplace("Down Idle", downIdle);
			animations.emplace("Down Left Idle", downLeftIdle);
			animations.emplace("Left Idle", leftIdle);
			animations.emplace("Up Left Idle", upLeftIdle);
			animations.emplace("Up Idle", upIdle);
			animations.emplace("Down Right Idle", downRightIdle);
			animations.emplace("Right Idle", rightIdle);
			animations.emplace("Up Right Idle", upRightIdle);
			//
			Animation downCast = Animation(2, 6, 180);
			Animation downLeftCast = Animation(5, 6, 180);
			Animation leftCast = Animation(8, 6, 180);
			Animation upLeftCast = Animation(11, 6, 180);
			Animation upCast = Animation(14, 6, 180);
			Animation downRightCast = Animation(17, 6, 180);
			Animation rightCast = Animation(20, 6, 180);
			Animation upRightCast = Animation(23, 6, 180);
			animations.emplace("Down Cast", downCast);
			animations.emplace("Down Left Cast", downLeftCast);
			animations.emplace("Left Cast", leftCast);
			animations.emplace("Up Left Cast", upLeftCast);
			animations.emplace("Up Cast", upCast);
			animations.emplace("Down Right Cast", downRightCast);
			animations.emplace("Right Cast", rightCast);
			animations.emplace("Up Right Cast", upRightCast);
			//
			play("Down Idle");
		}
		setText(id);

	}
	SpriteComponent(std::string id, bool isAnimated, int spellFacing)
	{
		// 0 = up, 1 = upRight, 2 = right, 3= downRight, 4 = down, 5 = down left, 6 = left, 7 = up left
		animated = isAnimated;

		switch (spellFacing)
		{
		case 0:
			spellAngle = 90;
			break;
		case 1:
			spellAngle = 135;
			break;
		case 2:
			spellAngle = 180;
			break;
		case 3:
			spellAngle = 225;
			break;
		case 4:
			spellAngle = 270;
			break;
		case 5:
			spellAngle = 315;
			break;
		case 6:
			spellAngle = 0;
			break;
		case 7:
			spellAngle = 45;
			break;
		default:
			spellAngle = 0;
			break;
		}
		Animation fireball = Animation(0, 4, 50);
		animations.emplace("fireball", fireball);
		play("fireball");
		setText(id);

	}
	~SpriteComponent()
	{
	}

	void initialize() override
	{
		movement = &entity->getComponent<MovementComponent>();
		srcRect.x = srcRect.y = 0;
		srcRect.w = movement->width;
		srcRect.h = movement->height;;
		destRect.w = movement->width * movement->scale;
		destRect.h = movement->height * movement->scale;
	}

	void updateLoop() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animationIndex * movement->height;
		
		destRect.x = static_cast<int>(movement->position.x);
		destRect.y = static_cast<int>(movement->position.y);
		destRect.w = movement->width * movement->scale;
		destRect.h = movement->height * movement->scale;
	}
	
	void drawLoop() override
	{
		if (texture == Game::objects->GetTexture("fireball"))
		{
			SDL_Point center;
			center.x = destRect.w / 4.0f;
			center.y = destRect.h / 2.0f;
			
			TextureManager::DrawRotate(texture, srcRect, destRect, spellAngle, &center);
		}
		else 
		{
			TextureManager::Draw(texture, srcRect, destRect);
		}
		
	}
	void play(const char* animationName)
	{
		frames = animations[animationName].frames;
		animationIndex = animations[animationName].index;
		speed = animations[animationName].speed;
	}
	void setText(std::string id)
	{
		texture = Game::objects->GetTexture(id);
	}

};