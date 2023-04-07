#include <SDL2/SDL.h>

SDL_Window *init(char *title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return NULL;
	}

	SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED, width,
										  height, SDL_WINDOW_SHOWN);
	if(window == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return NULL;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return NULL;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	return window;
}
