#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
	MovementComponent* movement;
	SpriteComponent* sprite;
	Game* game;

	int diagonalTimer = 5;
	int timerMax = 5;
	bool down;
	bool up;
	bool right;
	bool left;
	int facing;
	float mana = 50;
	float maxMana = 50;
	bool casting = false;
	float castingTime;
	float maxCastingTime = 30;
	int spellFacing;
	float singleDirectionSpeed = 1.4142136;
	
	// 0 = up, 1 = upRight, 2 = right, 3= downRight, 4 = down, 5 = down left, 6 = left, 7 = up left

	void initialize() override
	{
		movement = &entity->getComponent<MovementComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void updateLoop() override
	{
		diagonalTimer++;
		//mana regen
		if (mana < maxMana)
		{
			mana++;
		}
		//casting Timer
		if (casting == true)
		{
			castingTime+=1;
			if (castingTime == maxCastingTime)
			{
				game->CastSpell(true, spellFacing);
				casting = false;
			}
		}
		//all up movements
		if (up == true && down != true)
		{
			if (casting == false)
			{
				movement->velocity.y = -1;
			}
			if (right == true && left != true)
			{
				if (casting == false)
				{
					movement->velocity.x = 1;
					sprite->play("Up Right");
				}
				diagonalTimer = 0;
				facing = 1;
			}
			else if (left == true && right != true)
			{
				if (casting == false)
				{
					movement->velocity.x = -1;
					sprite->play("Up Left");
				}
				diagonalTimer = 0;
				facing = 7;
			}
			else
			{
				if (diagonalTimer < timerMax && casting == false)
				{
					switch (facing)
					{
					case 1:
						sprite->play("Up Right");
						break;
					case 7:
						sprite->play("Up Left");
						break;
					}
				}
				else
				{
					if (casting == false)
					{
						movement->velocity.y = -singleDirectionSpeed;
						movement->velocity.x = 0;
						sprite->play("Up");
					}
					facing = 0;
				}
			}
		}
		//all down movements
		if (down == true && up != true)
		{
			if (casting == false)
			{
				movement->velocity.y = 1;
			}
			if (right == true && left != true)
			{
				if (casting == false)
				{
					movement->velocity.x = 1;
					sprite->play("Down Right");
				}
				diagonalTimer = 0;
				facing = 3;
			}
			else if (left == true && right != true)
			{
				if (casting == false) 
				{
					movement->velocity.x = -1;
					sprite->play("Down Left");
					
				}
				diagonalTimer = 0;
				facing = 5;
			}
			else
			{
				if (diagonalTimer < timerMax && casting == false)
				{
					switch (facing)
					{
					case 3:
						sprite->play("Down Right");
						break;
					case 5:
						sprite->play("Down Left");
						break;
					}
				}
				else
				{
					if (casting == false)
					{
						movement->velocity.y = singleDirectionSpeed;
						movement->velocity.x = 0;
						sprite->play("Down");
					}
					facing = 4;
				}
			}
		}
		//handling all non vertical directions
		else if (down != true && up != true)
		{
			movement->velocity.y = 0;
			//handling right facing directions
			if (right == true && left != true)
			{
				if (diagonalTimer < timerMax && casting == false)
				{
					switch (facing)
					{
					case 1:
						sprite->play("Up Right");
						break;
					case 3:
						sprite->play("Down Right");
						break;
					}
				}
				else
				{
					if (casting == false)
					{
						movement->velocity.x = singleDirectionSpeed;
						sprite->play("Right");
					}
					facing = 2;
				}
			}
			//handling left facing directions
			else if (left == true && right != true)
			{
				if (diagonalTimer < timerMax && casting == false)
				{
					switch (facing)
					{
					case 7:
						sprite->play("Up Left");
						break;
					case 5:
						sprite->play("Down Left");
						break;
					}
				}
				else
				{
					if (casting == false)
					{
						movement->velocity.x = -singleDirectionSpeed;
						sprite->play("Left");
					}
					facing = 6;
				}
			}
		}

		// Idle Animation Handling
		if (left == false && right == false && up == false && down == false && casting == false)
		{
			movement->velocity.x = 0;
			movement->velocity.y = 0;
			switch (facing)
			{
			case 1:
				sprite->play("Up Right Idle");
				break;
			case 3:
				sprite->play("Down Right Idle");
				break;
			case 5:
				sprite->play("Down Left Idle");
				break;
			case 7:
				sprite->play("Up Left Idle");
				break;
			case 0:
				sprite->play("Up Idle");
				break;
			case 2:
				sprite->play("Right Idle");
				break;
			case 4:
				sprite->play("Down Idle");
				break;
			case 6:
				sprite->play("Left Idle");
				break;
			default:
				break;
			}
		}
		//handling human reflex accounting timer
		if (diagonalTimer == 6)
		{
			diagonalTimer = 5;
		}
		//handling all game input
		if (Game::event.type == SDL_KEYDOWN && Game::event.key.state == SDL_PRESSED)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				up = true;
				break;
			case SDLK_s:
				down = true;
				break;
			case SDLK_a:
				left = true;
				break;
			case SDLK_d:
				right = true;
				break;
			case SDLK_BACKSPACE:
				spellBool();
				break;
			default:
				break;
			}
		}
		if (Game::event.type == SDL_KEYUP && Game::event.key.state == SDL_RELEASED)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				up = false;
				break;
			case SDLK_s:
				down = false;
				break;
			case SDLK_a:	
				left = false;
				break;
			case SDLK_d:
				right = false;
				break;
			default:
				break;
			}
		}

	}
	void spellBool()
	{
		if (mana == maxMana && casting == false)
		{
			mana = 0;
			castingTime = 0;
			casting = true;
		}
		if (casting == true)
		{
			spellFacing = facing;
			movement->velocity.x = 0;
			movement->velocity.y = 0;
			switch (facing)
			{
			case 1:
				sprite->play("Up Right Cast");
				break;
			case 3:
				sprite->play("Down Right Cast");
				break;
			case 5:
				sprite->play("Down Left Cast");
				break;
			case 7:
				sprite->play("Up Left Cast");
				break;
			case 0:
				sprite->play("Up Cast");
				break;
			case 2:
				sprite->play("Right Cast");
				break;
			case 4:
				sprite->play("Down Cast");
				break;
			case 6:
				sprite->play("Left Cast");
				break;
			}
		}
	}
};
