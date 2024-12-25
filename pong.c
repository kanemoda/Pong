#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 640
#define HEIGHT 480
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 200
#define MOVEMET_SPEED 10
#define BALL_SPEED 7

float random_nonzero()
{
	float random_number;
	do
	{
		random_number = (float)rand() / RAND_MAX * 2.0f - 1.0f; // Generate between -1 and 1
	} while (random_number > -0.3f && random_number < 0.3f); // Ensure it's not 0
	return random_number;
}

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

void moveBall(SDL_Rect *ball, int *ballxSpeed, int *ballySpeed, SDL_Rect *pl1, SDL_Rect *pl2)
{
	ball->x += *ballxSpeed;
	ball->y += *ballySpeed;

	// Bounce off the top and bottom walls
	if (ball->y <= 0 || ball->y + 15 >= HEIGHT)
	{
		*ballySpeed *= -1;
	}

	// Paddle Collision Detecection for player 1
	if (ball->x <= pl1->x + pl1->w &&
		ball->x + ball->w >= pl1->x &&
		ball->y <= pl1->y + pl1->h &&
		ball->y + ball->h >= pl1->y)
	{
		*ballxSpeed = abs(*ballxSpeed);
		ball->x = pl1->x + pl1->w;

		if (*ballxSpeed > 0)
		{
			*ballxSpeed += 0.3f;
		}
		else
		{
			*ballxSpeed += -0.3f;
		}

		// Calculate collision point and adjust Y velocity
		float paddleCenter = pl1->y + pl1->h / 2.0f;
		float ballCenter = ball->y + ball->h / 2.0f;
		float collisionFactor = (ballCenter - paddleCenter) / (pl1->h / 2.0f); // Normalize between -1 and 1
		*ballySpeed = (int)(collisionFactor * BALL_SPEED);
		if (*ballySpeed == 0)
		{
			if (*ballxSpeed > 0)
			{
				*ballySpeed = 1;
			}
			else
			{
				*ballySpeed = -1;
			}
		}
	}

	// Paddle Collision Detecection for player 2
	if (ball->x + ball->w >= pl2->x &&
		ball->x <= pl2->x + pl2->w &&
		ball->y <= pl2->y + pl2->h &&
		ball->y + ball->h >= pl2->y)
	{
		*ballxSpeed = -abs(*ballxSpeed);
		ball->x = pl2->x - ball->w;
		if (*ballxSpeed > 0)
		{
			*ballxSpeed += 1.3f;
		}
		else
		{
			*ballxSpeed += -1.3f;
		}

		// Calculate collision point and adjust Y velocity
		float paddleCenter = pl2->y + pl2->h / 2.0f;
		float ballCenter = ball->y + ball->h / 2.0f;
		float collisionFactor = (ballCenter - paddleCenter) / (pl2->h / 2.0f); // Normalize between -1 and 1
		*ballySpeed = (int)(collisionFactor * BALL_SPEED);
		if (*ballySpeed == 0)
		{
			if (*ballxSpeed > 0)
			{
				*ballySpeed = 1;
			}
			else
			{
				*ballySpeed = -1;
			}
		}
	}

	if (ball->x <= 0 || ball->x + ball->w >= WIDTH)
	{
		printf("Score\n");
		ball->x = WIDTH / 2 - ball->w / 2;
		ball->y = HEIGHT / 2 - ball->h / 2;
		*ballxSpeed = random_nonzero() * BALL_SPEED;
		*ballySpeed = random_nonzero() * BALL_SPEED;
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
	srand((unsigned int)time(NULL));
	int ballxSpeed = random_nonzero() * BALL_SPEED;
	int ballySpeed = random_nonzero() * BALL_SPEED;

	SDL_InitSubSystem(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Surface *surface = SDL_GetWindowSurface(window);

	Uint32 backgroundcolor = SDL_MapRGB(surface->format, 0, 0, 0);
	Uint32 color = SDL_MapRGB(surface->format, 255, 255, 255);

	// Player1 Initializations
	SDL_Rect pl1 = {40, 40, PLAYER_WIDTH, PLAYER_HEIGHT};
	SDL_FillRect(surface, &pl1, color);
	// Player2 Initializations
	SDL_Rect pl2 = {WIDTH - 2 * PLAYER_WIDTH, 240, PLAYER_WIDTH, PLAYER_HEIGHT};
	SDL_FillRect(surface, &pl2, color);

	// Ball
	SDL_Rect ball = {WIDTH / 2, HEIGHT / 2, 15, 15};
	SDL_FillRect(surface, &ball, color);

	// First Window Update
	SDL_UpdateWindowSurface(window);

	// Required fields for objects
	int running = 1;
	int moveUp1 = 0, moveDown1 = 0;
	int moveUp2 = 0, moveDown2 = 0;

	// Main Event Loop
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
		SDL_FillRect(surface, &ball, backgroundcolor);

		movePlayer(&pl1, moveUp1, moveDown1);
		movePlayer(&pl2, moveUp2, moveDown2);
		moveBall(&ball, &ballxSpeed, &ballySpeed, &pl1, &pl2);

		// Redraw Paddle
		SDL_FillRect(surface, &pl1, color);
		SDL_FillRect(surface, &pl2, color);
		SDL_FillRect(surface, &ball, color);
		SDL_UpdateWindowSurface(window);
		SDL_Delay(10);
	}
}
