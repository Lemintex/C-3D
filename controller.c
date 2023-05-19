#include "controller.h"
/*
 * 1 = forward
 * 2 = backwards
 * 3 = left
 * 4 = right
 * 5 = ascend
 * 6 = descend
 */
	static unsigned short keyState = 0;
void handleKeyboardInput(SDL_Event* e) {
	int bit = 0;
	switch (e->key.keysym.sym) {
		case SDLK_w:
			bit = 1;
			break;
		
		case SDLK_s:
			bit = 2;
			break;
		
		case SDLK_a:
			bit = 3;
			break;
		
		case SDLK_d:
			bit = 4;
			break;
		
		case SDLK_r:
			bit = 5;
			break;
		
		case SDLK_f:
			bit = 6;
			break;
		default:
			break;
	}

	keyState ^= 1UL << bit;
}

void updateCamera() {
	extern vec3d_t camera;
	if (getBit(&keyState, 1)) camera.z += 0.01;
	if (getBit(&keyState, 2)) camera.z -= 0.01;
	if (getBit(&keyState, 3)) camera.x += 0.01;
	if (getBit(&keyState, 4)) camera.x -= 0.01;
	if (getBit(&keyState, 5)) camera.y += 0.01;
	if (getBit(&keyState, 6)) camera.y -= 0.01;
}

int getBit(unsigned short* keystate, int n) {
	return (*keystate >> n) & 1U;
}

void keyboard_flipBit(short* keyState);
//void handleKeyboardInput(SDL_KeyboardEvent* type) {
//	static short keyState = 0;
//	switch (type->state) {
//		case SDL_KEYDOWN:
//			onKeyPressed();
//			break;
//		case SDL_KEYUP:
//
//			break;
//
//		default:
//			break;
//	}
//}
//
//void onKeyPressed(SDL_Keycode* key) {
//
//}
//
//void onKeyReleased(SDL_Keycode* key) {
//
//}
//
////void handleMouseInput();
