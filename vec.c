#include "vec.h"
vec3d_t vec3_add(vec3d_t* v1, vec3d_t* v2) {
	return (vec3d_t){v1->x + v2->x, v1->y + v2->y, v1->z + v2->z, 1};
}

vec3d_t vec3_sub(vec3d_t* v1, vec3d_t* v2) {
	return (vec3d_t){v1->x - v2->x, v1->y - v2->y, v1->z - v2->z, 1};
}

vec3d_t vec3_mul(vec3d_t* v, float k) {
	return (vec3d_t){v->x * k, v->y * k, v->z * k, 1};
}

vec3d_t vec3_div(vec3d_t* v, float k) {
	return (vec3d_t){v->x / k, v->y / k, v->z / k, 1};
}

float vec3_dot(vec3d_t* v1, vec3d_t* v2) {
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

float vec3_length(vec3d_t* v) {
	return sqrtf(vec3_dot(v, v));
}

vec3d_t vec3_normal(vec3d_t* v) {
	float l = vec3_length(v);
	return (vec3d_t){v->x / l, v->y / l, v->z / l, 1};
}

vec3d_t vec3_cross(vec3d_t* v1, vec3d_t* v2) {
	vec3d_t vec;
	vec.x = v1->y * v2->z - v1->z * v2->y;
	vec.y = v1->z * v2->x - v1->x * v2->z;
	vec.z = v1->x * v2->y - v1->y * v2->x;
	vec.w = 1;
	return vec;
}

vec3d_t vec3_mul_mat4(vec3d_t* v, matrix_4x4_t* m) {
	vec3d_t vec;
	vec.x = v->x * m->m[0][0] + v->y * m->m[1][0] + v->z * m->m[2][0] + v->w * m->m[3][0];
	vec.y = v->x * m->m[0][1] + v->y * m->m[1][1] + v->z * m->m[2][1] + v->w * m->m[3][1];
	vec.z = v->x * m->m[0][2] + v->y * m->m[1][2] + v->z * m->m[2][2] + v->w * m->m[3][2];
	vec.w = v->x * m->m[0][3] + v->y * m->m[1][3] + v->z * m->m[2][3] + m->m[3][3];
	return vec;
}
