#include "controller.h"
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
void handleKeyboardInput(SDL_Event *e, unsigned short *mov)
{
	int bit = 0;
	switch (e->key.keysym.sym)
	{
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

	if (e->key.repeat)
	{
		return;
	}
	*mov ^= 1UL << bit;
}

void keyboard_flipBit(unsigned short *keyState);
