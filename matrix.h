#ifndef _MATRIX_H
#define _MATRIX_H

#include "mesh.h"
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

typedef struct matrix_4x4 {
	float m[4][4];
} matrix_4x4_t;

matrix_4x4_t* CreateEmptyMatrix();

matrix_4x4_t* GetTranformationMatrix(float width, float height);

void MultiplyMatrixByVector(vec3d_t in, vec3d_t* out, matrix_4x4_t* mat);

#endif
