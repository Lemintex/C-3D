#ifndef _3DMATH_H_
#define _3DMATH_H_

#include <math.h>

typedef struct matrix_4x4 {
	float m[4][4];
} matrix_4x4_t;

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

// VEC3D
vec3d_t vec3_add(vec3d_t* v1, vec3d_t* v2);

vec3d_t vec3_sub(vec3d_t* v1, vec3d_t* v2);

vec3d_t vec3_mul(vec3d_t* v1, float k);

vec3d_t vec3_div(vec3d_t* v1, float k);

float vec3_dot(vec3d_t* v1, vec3d_t* v2);

float vec3_length(vec3d_t* v);

vec3d_t vec3_normal(vec3d_t* v);

vec3d_t vec3_cross(vec3d_t* v1, vec3d_t* v2);

vec3d_t vec3_mul_mat4(vec3d_t* v, matrix_4x4_t* m);

vec3d_t vec3_intersectPlsne(vec3d_t* planePoint, vec3d_t* planeNormal, vec3d_t* lineStart, vec3d_t* lineEnd); 

// MAT4
matrix_4x4_t matrix_identity();

matrix_4x4_t matrix_rotationX(float angleRad);

matrix_4x4_t matrix_rotationY(float angleRad);

matrix_4x4_t matrix_rotationZ(float angleRad);

matrix_4x4_t matrix_translation(float x, float y, float z);

matrix_4x4_t matrix_projection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);

matrix_4x4_t matrix_multiplyMatrix(matrix_4x4_t* m1, matrix_4x4_t* m2);

matrix_4x4_t matrix_pointAt(vec3d_t* pos, vec3d_t* target, vec3d_t* up);
 
matrix_4x4_t matrix_quickInverse(matrix_4x4_t* mat);
#endif
