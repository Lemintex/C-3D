#include "camera.h"

extern camera_t camera;
extern options_t options;

void camera_update(double delta_time)
{
  unsigned short movement_speed = options.movement_speed;
	camera.look_dir = vec3_normal(&camera.look_dir);
	vec3d_t forward = vec3_mul(&camera.look_dir, delta_time * movement_speed);

	if (camera_get_movement_bit(1))
		camera.pos = vec3_add(&camera.pos, &forward);
	if (camera_get_movement_bit(2))
		camera.pos = vec3_sub(&camera.pos, &forward);
	if (camera_get_movement_bit(3))
		camera.pos.x += delta_time * movement_speed;
	if (camera_get_movement_bit(4))
		camera.pos.x -= delta_time * movement_speed;
	if (camera_get_movement_bit(5))
		camera.pos.y += delta_time * movement_speed;
	if (camera_get_movement_bit(6))
		camera.pos.y -= delta_time * movement_speed;
	if (camera_get_movement_bit(7))
		camera.yaw += delta_time;
	if (camera_get_movement_bit(8))
		camera.yaw -= delta_time;
}

int camera_get_movement_bit(int n)
{
	return (camera.mov >> n) & 1U;
}
