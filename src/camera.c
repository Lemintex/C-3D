#include "camera.h"

extern camera_t camera;

void camera_update(double deltaTime)
{
	camera.lookDir = vec3_normal(&camera.lookDir);
	vec3d_t forward = vec3_mul(&camera.lookDir, deltaTime * 1000);

	if (camera_getMovementBit(1))
		camera.pos = vec3_add(&camera.pos, &forward);
	if (camera_getMovementBit(2))
		camera.pos = vec3_sub(&camera.pos, &forward);
	if (camera_getMovementBit(3))
		camera.pos.x += deltaTime * 1000;
	if (camera_getMovementBit(4))
		camera.pos.x -= deltaTime * 1000;
	if (camera_getMovementBit(5))
		camera.pos.y += deltaTime * 1000;
	if (camera_getMovementBit(6))
		camera.pos.y -= deltaTime * 1000;
	if (camera_getMovementBit(7))
		camera.yaw += deltaTime;
	if (camera_getMovementBit(8))
		camera.yaw -= deltaTime;
}

int camera_getMovementBit(int n) { return (camera.mov >> n) & 1U; }
