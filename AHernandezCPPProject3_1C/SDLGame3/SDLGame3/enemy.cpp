#include "enemy.h"
#include <string>

Enemy::Enemy(SDL_Renderer* renderer, int speed, int maxH, int maxV, string dir, string type, int startX, int startY)
{
	enemyMovement = speed;
	maxHorizontalMovement = maxH;
	maxVerticalMovement = maxV;
	currentDirection = dir;
	enemyType = type;
	SDL_Surface* surface = IMG_Load("./Assets/enemy.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	posRect.x = startX;
	posRect.y = startY;
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;
	lastTime = 0;
}

void Enemy::Update(float deltaTime)
{
	currentTime = SDL_GetTicks();

	if (currentTime > lastTime + 500)
	{
		if ((currentDirection == "left") && (horizontalCounter < maxHorizontalMovement))
		{
			posRect.x -= enemyMovement;
			horizontalCounter++;

			if (horizontalCounter >= maxHorizontalMovement)
			{
				horizontalCounter = 0;
				if (enemyType == "CW")
				{
					currentDirection = "up";
				}
				else if (enemyType == "CCW")
				{
					currentDirection = "down";
				}
			}
		}
		else if ((currentDirection == "down") && (verticalCounter < maxVerticalMovement))
		{
			posRect.y += enemyMovement;
			verticalCounter++;

			if (verticalCounter >= maxVerticalMovement)
			{
				verticalCounter = 0;
				if (enemyType == "CW")
				{
					currentDirection = "left";
				}
				else if (enemyType == "CCW")
				{
					currentDirection = "right";
				}
			}
		}
		else if ((currentDirection == "right") && (horizontalCounter < maxHorizontalMovement))
		{
			posRect.x += enemyMovement;
			horizontalCounter++;

			if (horizontalCounter >= maxHorizontalMovement)
			{
				horizontalCounter = 0;
				if (enemyType == "CW")
				{
					currentDirection = "down";
				}
				else if (enemyType == "CCW")
				{
					currentDirection = "up";
				}
			}
		}
		else if ((currentDirection == "up") && (verticalCounter < maxVerticalMovement))
		{
			posRect.y -= enemyMovement;
			verticalCounter++;

			if (verticalCounter >= maxVerticalMovement)
			{
				verticalCounter = 0;
				if (enemyType == "CW")
				{
					currentDirection = "right";
				}
				else if (enemyType == "CCW")
				{
					currentDirection = "left";
				}
			}
		}

		lastTime = currentTime;
	}

}

void Enemy::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

void Enemy::Reset()
{

}

Enemy::~Enemy()
{
	//SDL_DestroyTexture(texture);
}