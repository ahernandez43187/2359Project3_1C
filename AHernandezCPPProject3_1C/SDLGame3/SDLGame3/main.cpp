#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>

using namespace std;

bool quit = false;
float deltaTime = 0.0f;
int thisTime = 0;
int lastTime = 0;

int playerMovement = 71;
SDL_Rect playerPos;

#include "enemy.h"
#include <vector>

vector<Enemy> enemyList;
int numberOfEnemies;

#include "coin.h"
vector<Coin> coinList;
int numberOfCoins;
Mix_Chunk* pickup;

int playerScore = 0;

int main(int argc, char* argv[])
{
	SDL_Window* window;
	SDL_Renderer* renderer = NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Dungeon Crawler", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 642, 358, SDL_WINDOW_OPENGL);

	if (window == NULL)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface* surface = IMG_Load("./Assets/level.png");
	SDL_Texture* bkgd;
	bkgd = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect bkgdPos;
	bkgdPos.x = 0;
	bkgdPos.y = 0;
	bkgdPos.w = 642;
	bkgdPos.h = 358;

	//
	surface = IMG_Load("./Assets/player.png");
	SDL_Texture* player;
	player = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	playerPos.x = 291;
	playerPos.y = 291;
	playerPos.w = 59;
	playerPos.h = 59;
	//

	SDL_Event event;

	const int mazeWidth = 9;
	const int mazeHeight = 5;

	string maze[mazeHeight][mazeWidth] =
	{
		{"O","O","O","O","O","O","O","O","O"},
		{"O","W","O","W","W","W","O","W","O"},
		{"O","O","O","O","W","O","O","O","O"},
		{"O","W","O","W","W","W","O","W","O"},
		{"O","O","O","O","P","O","O","O","O"}
	};

	int playerHorizontal = 4;
	int playerVertical = 4;

	//
	enemyList.clear();
	numberOfEnemies = 4;
	Enemy tempEnemy(renderer, 71, 2, 2, "left", "CCW", 575, 7);
	enemyList.push_back(tempEnemy);
	Enemy tempEnemy2(renderer, 71, 2, 2, "right", "CW", 7, 7);
	enemyList.push_back(tempEnemy2);
	Enemy tempEnemy3(renderer, 71, 2, 2, "right", "CW", 433, 149);
	enemyList.push_back(tempEnemy3);
	Enemy tempEnemy4(renderer, 71, 2, 2, "up", "CCW", 149, 291);
	enemyList.push_back(tempEnemy4);

	coinList.clear();
	numberOfCoins = 4;
	int totalCoins = 0;
	Coin tempCoin(renderer, 18, 18);
	coinList.push_back(tempCoin);
	Coin tempCoin2(renderer, 18, 302);
	coinList.push_back(tempCoin2);
	Coin tempCoin3(renderer, 586, 18);
	coinList.push_back(tempCoin3);
	Coin tempCoin4(renderer, 586, 302);
	coinList.push_back(tempCoin4);


	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	pickup = Mix_LoadWAV("./Assets/pickup.wav");

	while (!quit)
	{
		thisTime = SDL_GetTicks();
		deltaTime = (float)(thisTime - lastTime) / 1000;
		lastTime = thisTime;

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}

			switch (event.type)
			{
				case SDL_KEYUP:
					switch (event.key.keysym.sym)
					{
						case SDLK_RIGHT:
							if ((playerHorizontal + 1) < mazeWidth)
							{
								if (maze[playerVertical][playerHorizontal + 1] == "O")
								{
									maze[playerVertical][playerHorizontal + 1] = "P";
									maze[playerVertical][playerHorizontal] = "O";

									playerHorizontal += 1;
									playerPos.x += playerMovement;
								}
							}
							break;
						case SDLK_LEFT:
							if ((playerHorizontal - 1) >= 0)
							{
								if (maze[playerVertical][playerHorizontal - 1] == "O")
								{
									maze[playerVertical][playerHorizontal - 1] = "P";
									maze[playerVertical][playerHorizontal] = "O";

									playerHorizontal -= 1;
									playerPos.x -= playerMovement;
								}
							}
							break;
						case SDLK_UP:
							if ((playerVertical - 1) >= 0)
							{
								if (maze[playerVertical - 1][playerHorizontal] == "O")
								{
									maze[playerVertical - 1][playerHorizontal] = "P";
									maze[playerVertical][playerHorizontal] = "O";

									playerVertical -= 1;
									playerPos.y -= playerMovement;
								}
							}
							break;
						case SDLK_DOWN:
							if (playerVertical + 1 < mazeHeight)
							{
								if (maze[playerVertical + 1][playerHorizontal] == "O")
								{
									maze[playerVertical + 1][playerHorizontal] = "P";
									maze[playerVertical][playerHorizontal] = "O";

									playerVertical += 1;
									playerPos.y += playerMovement;
								}
							}
							break;
						default:
							break;
					}
			}
		}

		//
		for (int x = 0; x < numberOfEnemies; x++)
		{
			enemyList[x].Update(deltaTime);
		}

		for (int x = 0; x < enemyList.size(); x++)
		{
			if (SDL_HasIntersection(&playerPos, &enemyList[x].posRect))
			{
				cout << "Player was hit by an Enemy!" << endl;
				cout << "You have lost!" << endl;
			}
		}

		for (int x = 0; x < coinList.size(); x++)
		{
			if (SDL_HasIntersection(&playerPos, &coinList[x].posRect))
			{
				coinList[x].RemoveFromScreen();
				Mix_PlayChannel(-1, pickup, 0);
				totalCoins++;
				playerScore++;
				cout << "Total coins found: " << totalCoins << endl;
				cout << "Player score: " << playerScore << endl;
				if (totalCoins == 4)
				{
					cout << "You have won!" << endl;
				}
			}
		}
		//

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);
		SDL_RenderCopy(renderer, player, NULL, &playerPos);

		for (int x = 0; x < numberOfCoins; x++)
		{
			coinList[x].Draw(renderer);
		}

		for (int x = 0; x < numberOfEnemies; x++)
		{
			enemyList[x].Draw(renderer);
		}

		SDL_RenderPresent(renderer);

	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}