#include "camera.h"
#include <stdio.h>

extern camera_t camera;
extern options_t options;
extern double delta_time;

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
void camera_update() {
  camera_move();
  camera_look();
}

void camera_move() {
  unsigned short movement_speed = options.movement_speed;
  camera.look_dir = vec3_normal(&camera.look_dir);
  vec3d_t forward = vec3_mul(&camera.look_dir, delta_time * movement_speed);

  // move forwards
  if (camera_get_movement_bit(1)) {
    camera.pos = vec3_add(&camera.pos, &forward);
  }

  // move backwards
  if (camera_get_movement_bit(2)) {
    camera.pos = vec3_sub(&camera.pos, &forward);
  }

  if (camera_get_movement_bit(3)) {
    camera.pos.x += delta_time * movement_speed;
  }
  if (camera_get_movement_bit(4)) {
    camera.pos.x -= delta_time * movement_speed;
  }
  if (camera_get_movement_bit(5)) {
    camera.pos.y += delta_time * movement_speed;
  }
  if (camera_get_movement_bit(6)) {
    camera.pos.y -= delta_time * movement_speed;
  }
}

void camera_look() {
  float mouse_sensitivity = .25;

  // look up
  // TODO
  if (camera_get_look_bit(1)) {

  }
  
  // look down
  // TODO
  if (camera_get_look_bit(2)) {

  }

  // look left
  // TODO
  if (camera_get_look_bit(3)) {
    camera.yaw += delta_time * camera.yaw_speed * mouse_sensitivity;
  // reset yaw speed
  // printf("RESET YAW SPEED");
  camera.yaw_speed = 0;
    // printf("CAMERA Left yaw speed: %d\n", camera.yaw_speed);
  } else {
    // printf("%s", "Not looking left\n");
  }

  // look right
  // TODO4
  if (camera_get_look_bit(4)) {
    camera.yaw -= delta_time * camera.yaw_speed * mouse_sensitivity;
  }

  // if (camera_get_movement_bit(7)) {
  //   camera.yaw += delta_time;
  // }
  // if (camera_get_movement_bit(8)) {
  //   printf("%s\n", "Yaw Decreased");
  //   camera.yaw -= delta_time;
  // }
  //
}

int camera_get_movement_bit(int n) { return (camera.mov >> n) & 1U; }

int camera_get_look_bit(int n) { return (camera.look >> n) & 1U; }
