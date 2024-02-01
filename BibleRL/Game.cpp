#pragma once
#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "Collision.h"
#include <windows.h>
#include "ObjectController.h"

Manager manager;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
std::vector<ColliderComponent*> Game::colliders;
ObjectController* Game::objects = new ObjectController(&manager);

auto& player(manager.addEntity());
auto& gameOver(manager.addEntity());
auto& timerMinutes(manager.addEntity());
auto& timerMiddle(manager.addEntity());
auto& timerSeconds(manager.addEntity());
auto& hpMax(manager.addEntity());
auto& hpCurrent(manager.addEntity());
auto& xpBarEmpty(manager.addEntity());
auto& xpBarFull(manager.addEntity());
auto& background(manager.addEntity());
auto& startButton(manager.addEntity());
auto& quit(manager.addEntity());
auto& resume(manager.addEntity());
auto& pause(manager.addEntity());

float gameTime = 0.0f;
float pauseTime = 0.0f;
float pauseInterval = 0.0f;
int upgradedType;
bool cast = 0;
bool xpDrop = false;
float enemySpawn = 100.0f;
float enemyDamage = 0.01f;
float standardHeal = 0.01f;
float damage = 150.0f;
int spellFacing = 0;
float xpGain = 77.8f;
float levelUpRequirement = 778;
bool levelUp = false;
float enemySpawnInterval = 200;
float enemySpeed = 1.0f;
float healthBarSize = 28.0f;
bool paused = true;
int menu = 1;
float totalGameTime = 0;
float startGameTime = 0;
int upgradeType1;
int upgradeType2;
int upgradeType3;
float spellRadiusSQRD = 1000;
float spellAOE = 1200;
bool upgradedPlayer = false;
Vector2D xpLocation = Vector2D(0, 0);
Vector2D pausedMovement = Vector2D(0, 0);



auto& players(manager.getGroup(Game::allPlayers));
auto& opposers(manager.getGroup(Game::allOpposers));
auto& powerUps(manager.getGroup(Game::allPowerUps));
auto& spells(manager.getGroup(Game::allSpells));
auto& labels(manager.getGroup(Game::allLabels));
auto& menus(manager.getGroup(Game::allMenus));
auto& upgrades(manager.getGroup(Game::allUpgrades));

Game::Game()
{}
Game::~Game()
{}

void Game::initialize(const char* title, int xPos, int yPos, int width, int height, bool fullscreen)
{
	//inits a new random seed
	srand(time(NULL));
	//sets makes the game not fullscreen unless fullscreen is passed in as true
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	//initializes the window and renderer that is needeed to draw the game
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		running = true;
	} 
	else 
	{
		running = false;
	}
	// inits TTF and outputs error if failed
	if (TTF_Init() == -1)
	{
		std::cout << "TTF failed to init" << std::endl;
	}
	//setting up textures
	objects->AddTexture("player", "assets/Mage-Sheet.png");
	objects->AddTexture("xp", "assets/xp.png");
	objects->AddTexture("enemy", "assets/enemy.png");
	objects->AddTexture("fireball", "assets/fireball.png");
	objects->AddTexture("hpMax", "assets/redBar.png");
	objects->AddTexture("hpCurrent", "assets/greenBar.png");
	objects->AddTexture("xpBarEmpty", "assets/xpBarEmpty.png");
	objects->AddTexture("xpBarFull", "assets/xpBarFull.png");
	objects->AddTexture("pause", "assets/pause.png");
	objects->AddTexture("quit", "assets/quit.png");
	objects->AddTexture("resume", "assets/resume.png");
	objects->AddTexture("start", "assets/start.png");
	objects->AddTexture("background", "assets/background.png");
	objects->AddTexture("aoe", "assets/aoe.png");
	objects->AddTexture("casting", "assets/casting.png");
	objects->AddTexture("damage", "assets/damage.png");
	objects->AddTexture("heal", "assets/heal.png");
	objects->AddTexture("health", "assets/health.png");
	objects->AddTexture("pathing", "assets/pathing.png");
	objects->AddTexture("shield", "assets/shield.png");

	//fonts
	objects->AddFont("Lato", "assets/Lato.ttf", 32);
	objects->AddFont("LatoLarge", "assets/Lato.ttf", 96);

	//menus
	background.addComponent<MovementComponent>(0, 0, 672, 800, 1);
	background.addComponent<SpriteComponent>("background", false);
	background.addComponent<MouseController>();
	//start
	startButton.addComponent<MovementComponent>(256, 272, 32, 96, 3);
	startButton.addComponent<SpriteComponent>("start", false);
	startButton.addGroup(allMenus);
	//quit
	quit.addComponent<MovementComponent>(256, 386, 32, 96, 3);
	quit.addComponent<SpriteComponent>("quit", false);
	quit.addGroup(allMenus);
	//resume
	resume.addComponent<MovementComponent>(256, 158, 32, 96, 3);
	resume.addComponent<SpriteComponent>("resume", false);
	resume.addGroup(allMenus);
	//pause
	pause.addComponent<MovementComponent>(752, 16, 32, 32, 1);
	pause.addComponent<SpriteComponent>("pause", false);
	pause.addGroup(allMenus);
}


void Game::eventLoop()
{
	//checks to see if the game needs to be closed
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		running = false;
		break;
	default:
		break;
	}
}


void Game::updateLoop()
{	
	if (paused == false)
	{
		//#########################################################################
		//game loop
		//#########################################################################
		
		Vector2D playerPosition = player.getComponent<MovementComponent>().position;
		Vector2D playerNormPosition = playerPosition;
		Vector2D enemyPos;
		gameTime += 1;
		
		if (player.getComponent<HealthComponent>().health < 100)
		{
			player.getComponent<HealthComponent>().health += standardHeal;
		}
		//timer parts dynamic positioning and updating
		totalGameTime = SDL_GetTicks() - startGameTime - pauseTime;
		timerMinutes.getComponent<UILabel>().position.x = 400 - timerMiddle.getComponent<UILabel>().position.w - timerMinutes.getComponent<UILabel>().position.w;
		timerMinutes.getComponent<UILabel>().SetLabelText(std::to_string((int)(totalGameTime / 1000) / 60), "Lato");
		timerSeconds.getComponent<UILabel>().position.x = timerMiddle.getComponent<UILabel>().position.w + 400;
		timerSeconds.getComponent<UILabel>().SetLabelText(std::to_string((int)(totalGameTime / 1000) % 60), "Lato");
		timerMiddle.getComponent<UILabel>().position.x = 400-timerMiddle.getComponent<UILabel>().position.w/2;
		timerMiddle.getComponent<UILabel>().SetLabelText(":", "Lato");
		//casting a spell
		if (cast == true)
		{
			cast = false;
			objects->CreateSpell(Vector2D(player.getComponent<MovementComponent>().position.x + 20.0f,
				player.getComponent<MovementComponent>().position.y + player.getComponent<MovementComponent>().height),
				5000.0f, player.getComponent<MovementComponent>().speed-0.5f, "fireball", spellFacing);
		}
		//create enemy
		if (gameTime >= enemySpawn)
		{
			int startingSide = rand() % 4;
			switch (startingSide)
			{
			case 0:
				enemyPos = Vector2D(-32.0f + rand() % 833, -32.0f);
				break;
			case 1:
				enemyPos = Vector2D(-32.0f, -32.0f + rand() % 673);
				break;
			case 2:
				enemyPos = Vector2D(800.0f, -32.0f + rand() % 673);
				break;
			case 3:
				enemyPos = Vector2D(-32.0f + rand() % 833, 640.0f);
				break;
			}
			Vector2D startDirection;
			startDirection.FindNormVector(playerNormPosition - enemyPos, 1.0f);
			objects->CreateEnemy(enemyPos, startDirection, enemySpeed, "enemy");
			enemySpawn = gameTime + enemySpawnInterval;
			enemySpawnInterval *= 0.995;
			enemyDamage *= 1.005;
			damage *= 0.995;
			enemySpeed *= 1.005;
		}
		hpMax.getComponent<MovementComponent>().position = Vector2D(player.getComponent<MovementComponent>().position.x + 4, player.getComponent<MovementComponent>().position.y + 4 + (player.getComponent<MovementComponent>().height * player.getComponent<MovementComponent>().scale));
		hpCurrent.getComponent<MovementComponent>().position = hpMax.getComponent<MovementComponent>().position;
		hpCurrent.getComponent<MovementComponent>().width = (healthBarSize) * (player.getComponent<HealthComponent>().health / 100);
		//leveling up
		if (xpBarFull.getComponent<MovementComponent>().width >= levelUpRequirement)
		{
			pauseInterval = SDL_GetTicks();
			menu = 4;
			paused = true;
			xpBarFull.getComponent<MovementComponent>().width = 0;
			levelUp = true;
			xpGain *= 0.9;
		}
		//refresh and update (after creating enemies and spells or else weird texture glitching
		manager.refresh();
		manager.updateLoop();
		//enemy handling
		int iterationCount = 5;
		float pushoutFactor = 0.05f / float(iterationCount);
		for (int iterationIndex = 0; iterationIndex < iterationCount; iterationIndex++)
		{
			for (auto& o : opposers)
			{
				//move towards player
				for (auto& o2 : opposers)
				{
					//skip self
					if (o == o2)
					{
						continue;
					}
					Vector2D circleO = o->getComponent<MovementComponent>().circlePos;
					Vector2D circleO2 = o2->getComponent<MovementComponent>().circlePos;
					//

					float pushout = 0.0f;
					Vector2D directionNorm;
					if (Collision::BoundingBallEnemy(o->getComponent<MovementComponent>(), o2->getComponent<MovementComponent>(), pushout, directionNorm))
					{
						o->getComponent<MovementComponent>().position.x += directionNorm.x * pushout * pushoutFactor;
						o->getComponent<MovementComponent>().position.y += directionNorm.y * pushout * pushoutFactor;
						o2->getComponent<MovementComponent>().position.x -= directionNorm.x * pushout * pushoutFactor;
						o2->getComponent<MovementComponent>().position.y -= directionNorm.y * pushout * pushoutFactor;
					}
				}
				//player enemy collison
				Vector2D circleO = o->getComponent<MovementComponent>().circlePos;
				Vector2D circleP = player.getComponent<MovementComponent>().circlePos;
				float pushout = 0.0f;
				Vector2D directionNorm;
				if (Collision::BoundingBallEnemy(o->getComponent<MovementComponent>(), player.getComponent<MovementComponent>(), pushout, directionNorm))
				{
					o->getComponent<MovementComponent>().position.x += directionNorm.x * pushout * pushoutFactor;
					o->getComponent<MovementComponent>().position.y += directionNorm.y * pushout * pushoutFactor;
					player.getComponent<HealthComponent>().health -= enemyDamage;
					if (player.getComponent<HealthComponent>().IsDead())
					{
						menu = 2;
						paused = true;
						break;
					}
				}
				o->getComponent<MovementComponent>().followPlayer(playerPosition);
			}
		}
		//#########################################################################################

		//collisions
		//player collision
		if (Collision::Background(player.getComponent<ColliderComponent>().collider) == 1)
		{
			player.getComponent<MovementComponent>().position = playerPosition;
		}
		//collect xp
		for (auto& pu : powerUps)
		{
			if (Collision::AABB(player.getComponent<ColliderComponent>().collider, pu->getComponent<ColliderComponent>().collider))
			{
				xpBarFull.getComponent<MovementComponent>().width += xpGain;
				pu->destroy();
			}
		}
		//spell colliding with enemies
		for (auto& s : spells)
		{
			for (auto& o : opposers)
			{
				if (Collision::BoundingBallSpell(s->getComponent<ColliderComponent>().collider, o->getComponent<ColliderComponent>().collider, spellRadiusSQRD))
				{
					o->getComponent<HealthComponent>().health -= damage;
					s->destroy();
					if (o->getComponent<HealthComponent>().IsDead())
					{
						xpLocation.x = o->getComponent<MovementComponent>().position.x + (o->getComponent<MovementComponent>().width / 2);
						xpLocation.y = o->getComponent<MovementComponent>().position.y + (o->getComponent<MovementComponent>().height / 2);
						if (xpLocation.x < 32)
						{
							xpLocation.x = 32;
						}
						else if (xpLocation.x > 760)
						{
							xpLocation.x = 760;
						}
						else if (xpLocation.y < 32)
						{
							xpLocation.y = 32;
						}
						else if (xpLocation.y > 600)
						{
							xpLocation.y = 600;
						}
						CreateXP(xpLocation);
						o->destroy();
					}
					for (auto& o2 : opposers)
					{
						//skip self
						if (o == o2)
						{
							continue;
						}
						if (Collision::BoundingBallSpell(s->getComponent<ColliderComponent>().collider, o2->getComponent<ColliderComponent>().collider, spellAOE))
						{
							o2->getComponent<HealthComponent>().health -= damage;
							if (o2->getComponent<HealthComponent>().IsDead())
							{
								xpLocation.x = o2->getComponent<MovementComponent>().position.x + (o->getComponent<MovementComponent>().width / 2);
								xpLocation.y = o2->getComponent<MovementComponent>().position.y + (o->getComponent<MovementComponent>().height / 2);
								if (xpLocation.x < 32)
								{
									xpLocation.x = 32;
								}
								else if (xpLocation.x > 760)
								{
									xpLocation.x = 760;
								}
								else if (xpLocation.y < 32)
								{
									xpLocation.y = 32;
								}
								else if (xpLocation.y > 600)
								{
									xpLocation.y = 600;
								}
								CreateXP(xpLocation);
								o2->destroy();
							}
						}
					}
				}
			}
		}
		//menu system
		if (MouseClick(pause.getComponent<MovementComponent>().position, pause.getComponent<MovementComponent>().width, pause.getComponent<MovementComponent>().height, pause.getComponent<MovementComponent>().scale))
		{
			pauseInterval = SDL_GetTicks();
			menu = 3;
			paused = true;
		}
	}
	else 
	{
		
		background.getComponent<MouseController>().mousePress = true;
		
		if (menu == 4)
		{ 
			player.getComponent<MovementComponent>().velocity = pausedMovement;
			for (auto& o : opposers)
			{
				
				o->getComponent<MovementComponent>().velocity = pausedMovement;
			}
			for (auto& s : spells)
			{
				s->getComponent<MovementComponent>().paused = paused;
			}
			pauseInterval = SDL_GetTicks();
			if (levelUp == true)
			{
				if (player.getComponent<KeyboardController>().maxMana == 1 and player.getComponent<KeyboardController>().maxCastingTime == 1)
				{
					upgradeType1 = rand() % 6;
					if (upgradeType1 == 1)
					{
						upgradeType1+=1;
					}
					upgradeType2 = rand() % 6;
					while (upgradeType2 == upgradeType1)
					{
						upgradeType2 = rand() % 6;
					}
					if (upgradeType2 == 1)
					{
						upgradeType2+=1;
					}
					upgradeType3 = rand() % 6;
					while (upgradeType3 == upgradeType1 or upgradeType3 == upgradeType2)
					{
						upgradeType3 = rand() % 6;
					}
					if (upgradeType3 == 1)
					{
						upgradeType3+=1;
					}
				}
				else
				{
					upgradeType1 = rand() % 7;
					upgradeType2 = rand() % 7;
					while (upgradeType2 == upgradeType1)
					{
						upgradeType2 = rand() % 7;
					}
					upgradeType3 = rand() % 7;
					while (upgradeType3 == upgradeType1 or upgradeType3 == upgradeType2)
					{
						upgradeType3 = rand() % 7;
					}
				}
				
				objects->CreateUpgrade(Vector2D(336, 96), upgradeType1);
				objects->CreateUpgrade(Vector2D(336, 256), upgradeType2);
				objects->CreateUpgrade(Vector2D(336, 416), upgradeType3);
				levelUp = false;
			}
			for (auto& u : upgrades)
			{
				if (MouseClick(u->getComponent<MovementComponent>().position, u->getComponent<MovementComponent>().width, u->getComponent<MovementComponent>().height, u->getComponent<MovementComponent>().scale))
				{
					upgradedType = u->getComponent<UpgradeComponent>().upgradeType;
					upgradedPlayer = true;
				}
			}
			if (upgradedPlayer == true)
			{
				
				switch (upgradedType)//upgradeType)
				{
				case 0:
					spellAOE *= 1.2;
					upgradedPlayer = false;
					break;
				case 1:
					if (player.getComponent<KeyboardController>().maxMana != 1)
					{
						player.getComponent<KeyboardController>().maxMana -= 1;
					}
					if (player.getComponent<KeyboardController>().maxCastingTime != 1)
					{
						player.getComponent<KeyboardController>().maxCastingTime -= 1;
					}
					player.getComponent<KeyboardController>().mana -= 1;	
					player.getComponent<KeyboardController>().castingTime -= 1;
					upgradedPlayer = false;
					break;
				case 2:
					damage *= 1.2;
					upgradedPlayer = false;
					break;
				case 3:
					standardHeal *= 1.2;
					upgradedPlayer = false;
					break;
				case 4:
					player.getComponent<HealthComponent>().health = 100;
					upgradedPlayer = false;
					break;
				case 5:
					player.getComponent<MovementComponent>().speed *= 1.1;
					upgradedPlayer = false;
					break;
				case 6:
					enemyDamage *= 0.9;
					upgradedPlayer = false;
					break;
				}
				for (auto& u : upgrades)
				{
					u->destroy();
				}
				for (auto& s : spells)
				{
					s->getComponent<MovementComponent>().paused = paused;
				}
				menu = 0;
				paused = false;
				upgradedPlayer = false;
				pauseTime += SDL_GetTicks() - pauseInterval;
			}
		}
		manager.refresh();
		manager.updateLoop();
		//menu (1 = start screen, 2 = dead, 3 = pause, 0 = nothing)
		if (menu == 1)
		{
			if (MouseClick(startButton.getComponent<MovementComponent>().position, startButton.getComponent<MovementComponent>().width, startButton.getComponent<MovementComponent>().height, startButton.getComponent<MovementComponent>().scale))
			{
				LoadGame();
				menu = 0;
				paused = false;
			}	
		}
		if (menu == 2)
		{
			player.getComponent<MovementComponent>().velocity = pausedMovement;
			gameOver.getComponent<UILabel>().position.x = 400 - gameOver.getComponent<UILabel>().position.w / 2;
			if (MouseClick(quit.getComponent<MovementComponent>().position, quit.getComponent<MovementComponent>().width, quit.getComponent<MovementComponent>().height, quit.getComponent<MovementComponent>().scale))
			{
				cleanLoop();
			}
		}
		else if (menu == 3)
		{
			player.getComponent<MovementComponent>().velocity = pausedMovement;
			for (auto& o : opposers)
			{
				o->getComponent<MovementComponent>().velocity = pausedMovement;
			}
			for (auto& s : spells)
			{ 
				s->getComponent<MovementComponent>().paused = paused;
			}
			if (MouseClick(resume.getComponent<MovementComponent>().position, resume.getComponent<MovementComponent>().width, resume.getComponent<MovementComponent>().height, resume.getComponent<MovementComponent>().scale))
			{
				menu = 0;
				pauseTime += SDL_GetTicks() - pauseInterval;
				paused = false;
				for (auto& s : spells)
				{
					s->getComponent<MovementComponent>().paused = false;
				}
			}
			if (MouseClick(quit.getComponent<MovementComponent>().position, quit.getComponent<MovementComponent>().width, quit.getComponent<MovementComponent>().height, quit.getComponent<MovementComponent>().scale))
			{
				cleanLoop();
			}
		}	
	}
	background.getComponent<MouseController>().mousePosition;
}
void Game::renderLoop()
{
	//renders everything to screen in order
	SDL_RenderClear(renderer);
	background.drawLoop();
	for (auto& pu : powerUps)
	{
		pu->drawLoop();
	}
	for (auto& o : opposers)
	{
		o->drawLoop();
	}
	for (auto& s : spells)
	{
		s->drawLoop();
	}
	for (auto& p : players)
	{
		p->drawLoop();
	}
	for (auto& l : labels)
	{
		l->drawLoop();
	}
	//menu (1 = start screen, 2 = dead, 3 = pause, 0 = nothing)
	switch (menu)
	{
	case 1:
		startButton.drawLoop();
		break;
	case 2:
		gameOver.drawLoop();
		quit.drawLoop();
		break;
	case 3:
		resume.drawLoop();
		quit.drawLoop();
		break;
	case 4:
		for (auto& u : upgrades) {
			u->drawLoop();
		}
		break;
	case 0: 
		pause.drawLoop();
		break;
	}
	gameOver.drawLoop();
	SDL_RenderPresent(renderer);
}

void Game::cleanLoop()
{
	//cleans the screen of any renders
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::CastSpell(bool castIn, int facing)
{
	//lets the game know if a spell has been cast, and in what direction
	spellFacing = facing;
	cast = castIn;
}

void Game::LoadGame()
{
	player.addComponent<MovementComponent>(368.0f, 272.0f, 36.0f, 32.0f, 2.0f, 2.0f);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(allPlayers);
	player.addComponent<HealthComponent>();
	player.getComponent<MovementComponent>().radius = 64.0f;
	player.getComponent<MovementComponent>().player = 16.0f;

	//
	SDL_Color white = { 255, 255, 255, 255 };
	gameOver.addComponent<UILabel>(0, 158, "Game Over!", "LatoLarge", white);
	gameOver.getComponent<UILabel>().position.x = 400 - gameOver.getComponent<UILabel>().position.w / 2;
	timerMinutes.addComponent<UILabel>(300, 10, "", "Lato", white);
	timerMinutes.addGroup(allLabels);
	timerMiddle.addComponent<UILabel>(400, 10, "", "Lato", white);
	timerMiddle.addGroup(allLabels);
	timerSeconds.addComponent<UILabel>(500, 10, "", "Lato", white);
	timerSeconds.addGroup(allLabels);

	//
	xpBarEmpty.addComponent<MovementComponent>(0, 640, 32, 800, 1);
	xpBarEmpty.addComponent<SpriteComponent>("xpBarEmpty", false);
	xpBarEmpty.addGroup(allLabels);

	xpBarFull.addComponent<MovementComponent>(11, 651, 10.0f, 1, 1.0f);
	xpBarFull.addComponent<SpriteComponent>("xpBarFull", false);
	xpBarFull.addGroup(allLabels);

	//
	hpMax.addComponent<MovementComponent>(player.getComponent<MovementComponent>().position.x, player.getComponent<MovementComponent>().position.y + player.getComponent<MovementComponent>().height, 3, healthBarSize, 2);
	hpMax.addComponent<SpriteComponent>("hpMax", false);
	hpMax.addGroup(allLabels);

	hpCurrent.addComponent<MovementComponent>(player.getComponent<MovementComponent>().position.x, player.getComponent<MovementComponent>().position.y + player.getComponent<MovementComponent>().height, 3, healthBarSize, 2);
	hpCurrent.addComponent<SpriteComponent>("hpCurrent", false);
	hpCurrent.addGroup(allLabels);
	//game timer starting time
	startGameTime = SDL_GetTicks();
}

bool Game::MouseClick(Vector2D button, float width, float height, float scale)
{
	if (background.getComponent<MouseController>().mousePress == true &&
		background.getComponent<MouseController>().mousePosition.x > button.x &&
		background.getComponent<MouseController>().mousePosition.x < button.x + width * scale &&
		background.getComponent<MouseController>().mousePosition.y > button.y &&
		background.getComponent<MouseController>().mousePosition.y < button.y + height * scale)
	{
		background.getComponent<MouseController>().mousePress = false;
		return true;
	}		
	else
	{
		return false;
	}
}

void Game::CreateXP(Vector2D xpLocation)
{
	objects->CreateXP(xpLocation, "xp");

}


