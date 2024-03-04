#include "3dmath.h"
#include "camera.h"
#include "controller.h"
#include "mesh.h"
#include "options.h"
#include "renderer.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>

void init();
void init_camera();
void init_options();

options_t options;
SDL_Renderer *renderer;
camera_t camera;
const int width = 1000, height = 1000;
float *depth_buffer;
double delta_time = 0;

int main(int argc, char **argv) {
  printf("You have entered %d arguments:\n", argc);
 
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
  Uint64 previous_frame_time = SDL_GetPerformanceCounter();

  init();

  depth_buffer = (float *)malloc((width * height) * sizeof(float));
  memset(depth_buffer, 0, width * height);

  char title[] = "C-3D";

  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("Error");
  }
  SDL_Window *window =
      SDL_CreateWindow(title, 0, 0, width, height, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Surface *screen = SDL_GetWindowSurface(window);

  mesh_t *ship = read_mesh_from_file("res/A001_Spyro.obj", 1);
  SDL_Surface *texture = IMG_Load("res/A001_Spyro.png");

  while (1) {
    SDL_Event event;

        bool mouse_moved = false;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        SDL_DestroyWindow(window);
        return 0;
      case SDL_KEYUP:
      case SDL_KEYDOWN:
        handle_keyboard_input(&event, &camera.mov);
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
          SDL_DestroyWindow(window);
          return 0;
        }
        break;
      case SDL_MOUSEMOTION:
        handle_mouse_input(&event, &camera.look);
          mouse_moved = true;
        break;
      }
    }
    
    // if the mouse want's moved, the user isn't looking with the camera and all look bits should be 0
    if (!mouse_moved) {
      camera.look &= 0;
    }

    Uint64 current_frame_time = SDL_GetPerformanceCounter();
    delta_time = (double)(current_frame_time - previous_frame_time) /
                 SDL_GetPerformanceFrequency();
    previous_frame_time = current_frame_time;

    camera_update();
    memset(depth_buffer, 0, width * height);
    for (int i = 0; i < width * height; i++) {
      depth_buffer[i] = 1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    draw_mesh(renderer, ship, texture);

    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
  }
}

void init() {
  init_camera();
  init_options();
}

void init_camera() {
  camera.pos = (vec3d_t){0, 0, 0, 1};
  camera.look_dir.up = (vec3d_t){0, 0, 1, 1};
  camera.look_dir.forward = (vec3d_t){1, 0, 0, 1};
  camera.look_dir.right = (vec3d_t){0, 1, 0, 1};
}

void init_options() {
  options.movement_speed = 0;
  options.display_type = WIREFRAME;
}
