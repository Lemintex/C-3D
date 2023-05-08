#ifndef _MATRIX_H
#define _MATRIX_H

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

typedef struct matrix_4x4 {
	float m[4][4];
} matrix_4x4_t;

matrix_4x4_t matrix_identity();

matrix_4x4_t matrix_rotationX(float angleRad);

matrix_4x4_t matrix_rotationY(float angleRad);

matrix_4x4_t matrix_rotationZ(float angleRad);

matrix_4x4_t matrix_translation(float x, float y, float z);

matrix_4x4_t matrix_projection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);

matrix_4x4_t matrix_multiplyMatrix(matrix_4x4_t* m1, matrix_4x4_t* m2);

matrix_4x4_t* CreateEmptyMatrix();

matrix_4x4_t* GetTranformationMatrix(float width, float height);

#endif
