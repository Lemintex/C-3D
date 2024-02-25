#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "3dmath.h"
#include "options.h"

typedef struct camera
{
	vec3d_t pos;
	vec3d_t look_dir;
	unsigned short mov;
	float yaw, pitch;
} camera_t;

void camera_update(double delta_time);

int camera_get_movement_bit(int n);

#endif
