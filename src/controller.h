#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <SDL2/SDL.h>
#include <stdio.h>
#include "options.h"

void handle_keyboard_input(SDL_Event *e, unsigned short *mov);

void handle_movement(SDL_Event *e, unsigned short *mov);

void handle_speed(SDL_Event *e);

void handle_render_mode(SDL_Event *e);
#endif
