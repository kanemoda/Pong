#include <stdio.h>
#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 200
#define MOVEMET_SPEED 10

void movePlayer(SDL_Rect *rect, int moveUp, int moveDown)
{
	if (moveDown)
	{
		rect->y += MOVEMET_SPEED;
	}

	if (moveUp)
	{
		rect->y -= MOVEMET_SPEED;
	}

	// Prevent Out OF Bounds Attempt
	if (rect->y < 0)
	{
		rect->y = 0;
	}
	if (rect->y + PLAYER_HEIGHT > HEIGHT)
	{
		rect->y = HEIGHT - PLAYER_HEIGHT;
	}
}

void handleKeyEvent(SDL_Event *event, int *moveUp, int *moveDown, SDL_Keycode upKey, SDL_Keycode downKey)
{
	if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == downKey)
		{
			*moveDown = 1;
		}
		if (event->key.keysym.sym == upKey)
		{
			*moveUp = 1;
		}
	}

	else if (event->type == SDL_KEYUP)
	{
		if (event->key.keysym.sym == downKey)
		{
			*moveDown = 0;
		}

		if (event->key.keysym.sym == upKey)
		{
			*moveUp = 0;
		}
	}
}

int main()
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Surface *surface = SDL_GetWindowSurface(window);

	Uint32 backgroundcolor = SDL_MapRGB(surface->format, 0, 0, 0);
	Uint32 color = SDL_MapRGB(surface->format, 255, 255, 255);

	SDL_Rect pl1 = {40, 40, PLAYER_WIDTH, PLAYER_HEIGHT};
	SDL_FillRect(surface, &pl1, color);

	SDL_Rect pl2 = {WIDTH - 2 * PLAYER_WIDTH, 240, PLAYER_WIDTH, PLAYER_HEIGHT};
	SDL_FillRect(surface, &pl2, color);

	SDL_UpdateWindowSurface(window);
	int running = 1;
	int moveUp1 = 0, moveDown1 = 0;
	int moveUp2 = 0, moveDown2 = 0;
	SDL_Event event;
	while (running)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
		{
			running = 0;
		}
		handleKeyEvent(&event, &moveUp1, &moveDown1, SDLK_w, SDLK_s);
		handleKeyEvent(&event, &moveUp2, &moveDown2, SDLK_UP, SDLK_DOWN);

		SDL_FillRect(surface, &pl1, backgroundcolor);
		SDL_FillRect(surface, &pl2, backgroundcolor);

		movePlayer(&pl1, moveUp1, moveDown1);
		movePlayer(&pl2, moveUp2, moveDown2);

		// Redraw Paddle
		SDL_FillRect(surface, &pl1, color);
		SDL_FillRect(surface, &pl2, color);

		/* if (event.key.keysym.sym == SDLK_DOWN)
		{
			SDL_FillRect(surface, &pl1, 0x00000000);
			pl1.y += MOVEMET_SPEED;
			SDL_FillRect(surface, &pl1, color);
		}
		if (event.key.keysym.sym == SDLK_UP)
		{
			SDL_FillRect(surface, &pl1, 0x00000000);
			pl1.y -= MOVEMET_SPEED;
			SDL_FillRect(surface, &pl1, color);
		} */
		SDL_UpdateWindowSurface(window);
		SDL_Delay(10);
	}
}
