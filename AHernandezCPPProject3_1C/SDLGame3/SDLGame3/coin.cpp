#include "coin.h"

Coin::Coin(SDL_Renderer* renderer, int startX, int startY)
{
	active = true;
	SDL_Surface* surface = IMG_Load("./Assets/coin.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	posRect.x = startX;
	posRect.y = startY;
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;
}

void Coin::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

void Coin::RemoveFromScreen()
{
	active = false;
	posRect.x = -2000;
	posRect.y = -2000;
}

Coin::~Coin()
{
	//SDL_DestroyTexture(texture);
}