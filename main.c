#include "3dmath.h"
#include "camera.h"
#include "controller.h"
#include "mesh.h"
#include "renderer.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>

SDL_Renderer* renderer;
camera_t camera;

int main() {
	camera.pos = (vec3d_t){0, 0, 0, 1};
	camera.lookDir = (vec3d_t){0, 0, 1, 1};
	char title[] = "Test";
	int width = 500, height = 500;
	if(SDL_Init(SDL_INIT_VIDEO)) {
		printf("Error");
	}
	SDL_Window* window = SDL_CreateWindow(title, 0, 0, width, height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface* screen = SDL_GetWindowSurface(window);
 
	mesh_t* ship = ReadMeshFromFile("axis.obj");
	while(1) {
		SDL_Event event;

		while(SDL_PollEvent(&event)) {
			switch( event.type ){
				case SDL_QUIT:
					SDL_DestroyWindow(window);
					return 0;
				case SDL_KEYUP:
				case SDL_KEYDOWN:
					handleKeyboardInput(&event, &camera.mov);
					if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
						SDL_DestroyWindow(window);
						return 0;
					}
			}
		}
		camera_update();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		DrawMesh(renderer, ship);
		
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);
	}
}
