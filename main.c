#include "mesh.h"
#include "renderer.h"

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>

int main() {
	char title[] = "Test";
	int width = 640, height = 480;
	if(SDL_Init(SDL_INIT_VIDEO)) {
		printf("Error");
	}
	SDL_Window* window = SDL_CreateWindow(title, 0, 0, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface* screen = SDL_GetWindowSurface(window);
 
	mesh_t* cube = CreateCube();
	while(1) {
		SDL_Event event;

		while(SDL_PollEvent(&event)) {
			switch( event.type ){
				case SDL_QUIT:
					return 0; 
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 0, 200, SDL_ALPHA_OPAQUE);
		DrawMesh(renderer, cube);
		
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);
	}
}
