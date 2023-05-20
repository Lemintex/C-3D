#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <SDL2/SDL.h>

#include "3dmath.h" // TEMPORARY

void handleKeyboardInput(SDL_Event* type);

void updateCamera();

int getBit(unsigned short* keystate, int n);

void onKeyPressed(SDL_Keycode* key);

void onKeyReleased(SDL_Keycode* key);

void handleMouseInput();

void keyboard_flipBit(unsigned short* keyState);
#endif
