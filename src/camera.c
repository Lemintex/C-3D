#include "camera.h"
#include "3dmath.h"
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
  vec3d_t forward_speed = vec3_mul(&camera.look_dir.forward, movement_speed / 100.0);
  vec3d_t right_speed = vec3_mul(&camera.look_dir.right, movement_speed / 100.0);
  vec3d_t up_speed = vec3_mul(&camera.look_dir.up, movement_speed / 100.0);

  camera.look_dir.forward = vec3_mul(&camera.look_dir.forward, movement_speed / 100.00);
  
  // FORWARD
  if (camera_get_movement_bit(1)) {
    camera.pos = vec3_add(&camera.pos, &forward_speed);
  }

  // BACK
  if (camera_get_movement_bit(2)) {
    camera.pos = vec3_sub(&camera.pos, &forward_speed);
  }

  // LEFT
  if (camera_get_movement_bit(3)) {
    camera.pos = vec3_sub(&camera.pos, &right_speed);
  }

  // RIGHT
  if (camera_get_movement_bit(4)) {
    camera.pos = vec3_add(&camera.pos, &right_speed);
  }

  // UP
  if (camera_get_movement_bit(5)) {
    camera.pos = vec3_add(&camera.pos, &up_speed);
  }

  // DOWN
  if (camera_get_movement_bit(6)) {
    camera.pos = vec3_sub(&camera.pos, &up_speed);
  }
}

void camera_look() {
  float mouse_sensitivity = .25;

  // look up
  if (camera_get_look_bit(1)) {
    camera.pitch -= delta_time * camera.pitch_speed * mouse_sensitivity;
    camera.pitch_speed = 0;
  }

  // look down
  if (camera_get_look_bit(2)) {
    camera.pitch += delta_time * camera.pitch_speed * mouse_sensitivity;
    camera.pitch_speed = 0;
  }

  // look left
  if (camera_get_look_bit(3)) {
    camera.yaw += delta_time * camera.yaw_speed * mouse_sensitivity;
    camera.yaw_speed = 0;
  }

  // look right
  if (camera_get_look_bit(4)) {
    camera.yaw -= delta_time * camera.yaw_speed * mouse_sensitivity;
    camera.yaw_speed = 0;
  }
}

int camera_get_movement_bit(int n) { return (camera.mov >> n) & 1U; }

int camera_get_look_bit(int n) { return (camera.look >> n) & 1U; }
