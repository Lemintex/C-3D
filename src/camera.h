#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "3dmath.h"
#include "options.h"

typedef struct look {
  vec3d_t forward;
  vec3d_t up;
  vec3d_t right;
} look_t;

typedef struct camera {
  vec3d_t pos;
  look_t look_dir;
  unsigned short mov;
  unsigned short look;
  float pitch, yaw;
  int pitch_speed, yaw_speed;
} camera_t;

void camera_update();

int camera_get_movement_bit(int n);

int camera_get_look_bit(int n);

void camera_move();

void camera_look();
#endif
