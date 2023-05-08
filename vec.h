#ifndef _VEC_H_
#define _VEC_H_

#include "matrix.h"
#include <math.h>

typedef struct {
	float x;
	float y;
	float z;
	float w;
} vec3d_t;

typedef struct {
	float x;
	float y;
} vec2d_t;

// these functions require an output address to be dedclared in the stack
// I did this to avoid memory leaks with forgetting to free heap memory
vec3d_t vec3_add(vec3d_t* v1, vec3d_t* v2);

vec3d_t vec3_sub(vec3d_t* v1, vec3d_t* v2);

vec3d_t vec3_mul(vec3d_t* v1, float k);

vec3d_t vec3_div(vec3d_t* v1, float k);

float vec3_dot(vec3d_t* v1, vec3d_t* v2);

float vec3_length(vec3d_t* v);

vec3d_t vec3_normal(vec3d_t* v);

vec3d_t vec3_cross(vec3d_t* v1, vec3d_t* v2);

vec3d_t vec3_mul_mat4(vec3d_t* v, matrix_4x4_t* m);
#endif
