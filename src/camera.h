#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "3dmath.h"

typedef struct camera
{
	vec3d_t pos;
	vec3d_t lookDir;
	unsigned short mov;
	float yaw, pitch;
} camera_t;

void camera_update(double deltaTime);

int camera_getMovementBit(int n);

#endif