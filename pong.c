#include <stdio.h>
#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 200
#define MOVEMET_SPEED 10

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
	int moveUp = 0, moveDown = 0;
	SDL_Event event;
	while (running)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
		{
			running = 0;
		}
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				moveDown = 1;
			}
			if (event.key.keysym.sym == SDLK_UP)
			{
				moveUp = 1;
			}
		}

		if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				moveDown = 0;
			}

			if (event.key.keysym.sym == SDLK_UP)
			{
				moveUp = 0;
			}
		}

		SDL_FillRect(surface, &pl1, backgroundcolor);

		if (moveDown)
		{
			pl1.y += MOVEMET_SPEED;
		}

		if (moveUp)
		{
			pl1.y -= MOVEMET_SPEED;
		}

		// Prevent Out OF Bounds Attempt
		if (pl1.y < 0)
		{
			pl1.y = 0;
		}
		if (pl1.y + PLAYER_HEIGHT > HEIGHT)
		{
			pl1.y = HEIGHT - PLAYER_HEIGHT;
		}

		// Redraw Paddle
		SDL_FillRect(surface, &pl1, color);

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
