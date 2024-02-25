#include "controller.h"

extern options_t options;

void handle_keyboard_input(SDL_Event *e, unsigned short *mov) {
  handle_movement(e, mov);
  handle_speed(e);
  handle_render_mode(e);
}
/*
 * 1 = forward
 * 2 = backwards
 * 3 = left
 * 4 = right
 * 5 = ascend
 * 6 = descend
 * 7 = yaw left
 * 8 = yaw right
 */
void handle_movement(SDL_Event *e, unsigned short *mov) {
	int bit = 0;
	switch (e->key.keysym.sym) {
	case SDLK_w:
		bit = 1;
		break;

	case SDLK_s:
		bit = 2;
		break;

	case SDLK_d:
		bit = 3;
		break;

	case SDLK_a:
		bit = 4;
		break;

	case SDLK_r:
		bit = 5;
		break;

	case SDLK_f:
		bit = 6;
		break;

	case SDLK_q:
		bit = 7;
		break;

	case SDLK_e:
		bit = 8;
		break;

	default:
		break;
	}

	if (e->key.repeat) {
		return;
	}
	*mov ^= 1UL << bit;
}

void handle_speed(SDL_Event *e) {
  switch (e->key.keysym.sym) {
    case SDLK_UP:
      options.movement_speed++;
      printf("Movement Speed: %hu\n", options.movement_speed);
      break;

    case SDLK_DOWN:
      if (options.movement_speed > 0) {
        options.movement_speed--;
        printf("Movement Speed: %hu\n", options.movement_speed);
      }
      break;
  }
}

void handle_render_mode(SDL_Event *e) {
  if (e->key.repeat || e->type == SDL_KEYDOWN) {
    return;
  }
  switch (e->key.keysym.sym) {
    case SDLK_TAB:
      switch (options.display_type) {
        case WIREFRAME:
          options.display_type = MONOCHROME;
          break;

        case MONOCHROME:
          options.display_type = TEXTURE;
          break;

        case TEXTURE:
          options.display_type = WIREFRAME;
          break;
      }
  }
}
