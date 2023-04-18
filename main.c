#include "mesh.h"
#include "renderer.h"

#include <SDL2/SDL_video.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>

SDL_Renderer* renderer;
vec3d_t camera;

int main() {
	camera = (vec3d_t){0, 0, 0};
	char title[] = "Test";
	int width = 640, height = 480;
	if(SDL_Init(SDL_INIT_VIDEO)) {
		printf("Error");
	}
	SDL_Window* window = SDL_CreateWindow(title, 0, 0, width, height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface* screen = SDL_GetWindowSurface(window);
 
	mesh_t* cube = CreateCube();
	while(1) {
		SDL_Event event;

		while(SDL_PollEvent(&event)) {
			switch( event.type ){
				case SDL_QUIT:
					SDL_DestroyWindow(window);
					return 0; 
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		DrawMesh(renderer, cube);
		
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);
	}
}
