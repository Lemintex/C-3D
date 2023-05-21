#include "camera.h"

extern camera_t camera;

void camera_update() {
	if (camera_getMovementBit(1)) camera.pos.z += 0.01;
	if (camera_getMovementBit(2)) camera.pos.z -= 0.01;
	if (camera_getMovementBit(3)) camera.pos.x += 0.01;
	if (camera_getMovementBit(4)) camera.pos.x -= 0.01;
	if (camera_getMovementBit(5)) camera.pos.y += 0.01;
	if (camera_getMovementBit(6)) camera.pos.y -= 0.01;
	if (camera_getMovementBit(7)) camera.yaw += 0.000001;
	if (camera_getMovementBit(8)) camera.yaw -= 0.000001;
}

int camera_getMovementBit(int n) {
	return (camera.mov >> n) & 1U;
}
