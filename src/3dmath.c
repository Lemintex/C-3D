#include "3dmath.h"

vec3d_t vec3_add(vec3d_t *v1, vec3d_t *v2) {
	return (vec3d_t){v1->x + v2->x, v1->y + v2->y, v1->z + v2->z, 1};
}

vec3d_t vec3_sub(vec3d_t *v1, vec3d_t *v2) {
	return (vec3d_t){v1->x - v2->x, v1->y - v2->y, v1->z - v2->z, 1};
}

vec3d_t vec3_mul(vec3d_t *v, float k) {
	return (vec3d_t){v->x * k, v->y * k, v->z * k, 1};
}

vec3d_t vec3_div(vec3d_t *v, float k) {
	return (vec3d_t){v->x / k, v->y / k, v->z / k, 1};
}

float vec3_dot(vec3d_t *v1, vec3d_t *v2) {
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

float vec3_length(vec3d_t *v) {
	return sqrtf(vec3_dot(v, v));
}

vec3d_t vec3_normal(vec3d_t *v) {
	float l = vec3_length(v);
	return (vec3d_t){v->x / l, v->y / l, v->z / l, 1};
}

vec3d_t vec3_cross(vec3d_t *v1, vec3d_t *v2) {
	vec3d_t vec;

	vec.x = v1->y * v2->z - v1->z * v2->y;
	vec.y = v1->z * v2->x - v1->x * v2->z;
	vec.z = v1->x * v2->y - v1->y * v2->x;
	vec.w = 1;
	return vec;
}

vec3d_t vec3_mul_mat4(vec3d_t *v, matrix_4x4_t *m) {
	vec3d_t vec;

	vec.x = v->x * m->m[0][0] + v->y * m->m[1][0] + v->z * m->m[2][0] + v->w * m->m[3][0];
	vec.y = v->x * m->m[0][1] + v->y * m->m[1][1] + v->z * m->m[2][1] + v->w * m->m[3][1];
	vec.z = v->x * m->m[0][2] + v->y * m->m[1][2] + v->z * m->m[2][2] + v->w * m->m[3][2];
	vec.w = v->x * m->m[0][3] + v->y * m->m[1][3] + v->z * m->m[2][3] + v->w * m->m[3][3];
	return vec;
}

vec3d_t vec3_intersect_plane(vec3d_t *plane_point, vec3d_t *plane_normal,
							               vec3d_t *line_start, vec3d_t *line_end, float *t) {
	// ensure the normal is normalised
	*plane_normal = vec3_normal(plane_normal);

	// calculate the direction of the line
	vec3d_t line_direction = vec3_sub(line_end, line_start);

	// calculate the dot product of planeNormal and lineDirection
	float dot_product = vec3_dot(plane_normal, &line_direction);

	// calculate the vector from the plane point to the line start
	vec3d_t plane_to_point = vec3_add(line_start, plane_point);

	// calculate the distance factor along the line where the intersection occurs
	*t = -vec3_dot(plane_normal, &plane_to_point) / dot_product;

	// calculate the coordinates of the intersection point
	vec3d_t distance_to_point = vec3_mul(&line_direction, *t);

	return vec3_add(line_start, &distance_to_point);
}

triangle_t triangle_mul_mat4(triangle_t *t, matrix_4x4_t *m) {
	triangle_t triangle;

	t->verts[0] = vec3_mul_mat4(&t->verts[0], m);
	t->verts[1] = vec3_mul_mat4(&t->verts[1], m);
	t->verts[2] = vec3_mul_mat4(&t->verts[2], m);
	return *t;
}

matrix_4x4_t matrix_identity() {
	matrix_4x4_t matrix = {0};

	matrix.m[0][0] = 1;
	matrix.m[1][1] = 1;
	matrix.m[2][2] = 1;
	matrix.m[3][3] = 1;
	return matrix;
}

matrix_4x4_t matrix_rotation_x(float angle_rad) {
	matrix_4x4_t matrix = {0};

	matrix.m[0][0] = 1;
	matrix.m[1][1] = cosf(angle_rad);
	matrix.m[1][2] = sinf(angle_rad);
	matrix.m[2][1] = -sinf(angle_rad);
	matrix.m[2][2] = cosf(angle_rad);
	matrix.m[3][3] = 1;
	return matrix;
}

matrix_4x4_t matrix_rotation_y(float angle_rad) {
	matrix_4x4_t matrix = {0};

	matrix.m[0][0] = cosf(angle_rad);
	matrix.m[0][2] = sinf(angle_rad);
	matrix.m[1][1] = 1;
	matrix.m[2][0] = -sinf(angle_rad);
	matrix.m[2][2] = cosf(angle_rad);
	matrix.m[3][3] = 1;
	return matrix;
}

matrix_4x4_t matrix_rotation_z(float angle_rad) {
	matrix_4x4_t matrix = {0};

	matrix.m[0][0] = cosf(angle_rad);
	matrix.m[0][1] = sinf(angle_rad);
	matrix.m[1][0] = -sinf(angle_rad);
	matrix.m[1][1] = cosf(angle_rad);
	matrix.m[2][2] = 1;
	matrix.m[3][3] = 1;
	return matrix;
}

matrix_4x4_t matrix_translation(float x, float y, float z) {
	matrix_4x4_t matrix = matrix_identity();

	matrix.m[3][0] = x;
	matrix.m[3][1] = y;
	matrix.m[3][2] = z;
	return matrix;
}

matrix_4x4_t matrix_projection(float f_fov_degrees, float f_aspect_ratio,
							                 float f_near, float f_far) {
	float f_fov_rad = 1 / tanf((f_fov_degrees / 2) / 180 * M_PI);
	matrix_4x4_t matrix = {0};

	matrix.m[0][0] = f_aspect_ratio * f_fov_rad;
	matrix.m[1][1] = f_fov_rad;
	matrix.m[2][2] = f_far / (f_far - f_near);
	matrix.m[2][3] = 1.0;
	matrix.m[3][2] = (-f_far * f_near) / (f_far - f_near);
	matrix.m[3][3] = 0.0;
	return matrix;
}

matrix_4x4_t matrix_multiply_matrix(matrix_4x4_t *m1, matrix_4x4_t *m2) {
	matrix_4x4_t matrix = {0};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix.m[i][j] = 0;
			for (int k = 0; k < 4; k++)
			{
				matrix.m[i][j] += m1->m[i][k] * m2->m[k][j];
			}
		}
	}
	return matrix;
}

matrix_4x4_t matrix_point_at(vec3d_t *pos, vec3d_t *target, vec3d_t *up) {
	vec3d_t new_forward = vec3_sub(pos, target);
	new_forward = vec3_normal(&new_forward);

	vec3d_t a = vec3_mul(&new_forward, vec3_dot(up, &new_forward));
	vec3d_t new_up = vec3_sub(up, &a);
	new_up = vec3_normal(&new_up);

	vec3d_t new_right = vec3_cross(&new_up, &new_forward);

	matrix_4x4_t matrix = {0};

	matrix.m[0][0] = new_right.x;
	matrix.m[0][1] = new_right.y;
	matrix.m[0][2] = new_right.z;
	matrix.m[0][3] = 0;

	matrix.m[1][0] = new_up.x;
	matrix.m[1][1] = new_up.y;
	matrix.m[1][2] = new_up.z;
	matrix.m[1][3] = 0;

	matrix.m[2][0] = new_forward.x;
	matrix.m[2][1] = new_forward.y;
	matrix.m[2][2] = new_forward.z;
	matrix.m[2][3] = 0;

	matrix.m[3][0] = pos->x;
	matrix.m[3][1] = pos->y;
	matrix.m[3][2] = pos->z;
	matrix.m[3][3] = 1;
	return matrix;
}

matrix_4x4_t matrix_quick_inverse(matrix_4x4_t *mat) { // Only for Rotation/Translation Matrices
	matrix_4x4_t m = *mat;
	matrix_4x4_t matrix;

	matrix.m[0][0] = m.m[0][0];
	matrix.m[0][1] = m.m[1][0];
	matrix.m[0][2] = m.m[2][0];
	matrix.m[0][3] = 0.0f;

	matrix.m[1][0] = m.m[0][1];
	matrix.m[1][1] = m.m[1][1];
	matrix.m[1][2] = m.m[2][1];
	matrix.m[1][3] = 0.0f;

	matrix.m[2][0] = m.m[0][2];
	matrix.m[2][1] = m.m[1][2];
	matrix.m[2][2] = m.m[2][2];
	matrix.m[2][3] = 0.0f;

	matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
