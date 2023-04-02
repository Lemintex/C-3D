#include "matrix.h"
matrix_4x4_t* CreateEmptyMatrix() {
	matrix_4x4_t* matrix = (matrix_4x4_t*)malloc(sizeof(matrix_4x4_t));
	return matrix;
}


matrix_4x4_t* GetTranformationMatrix(float width, float height) {
	matrix_4x4_t* matrix = CreateEmptyMatrix();
	float fNear = 0.1;
	float fFar = 1000;
	float fFov = 90;
	float fAspectRatio = height / width; 
	float fFovRad = 1.0 / tanf((fFov / 2) / 180 * M_PI);

	matrix->m[0][0] = fAspectRatio * fFovRad;
	matrix->m[1][1] = fFovRad;
	matrix->m[2][2] = fFar / (fFar - fNear);
	matrix->m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matrix->m[2][3] = 1.0;
	matrix->m[3][3] = 0.0; 
	return matrix;
}


void MultiplyMatrixByVector(vec3d_t* in, vec3d_t* out, matrix_4x4_t* mat) {
	out->x = in->x * mat->m[0][0] + in->y * mat->m[1][0] + in->z * mat->m[2][0] + mat->m[3][0];
	out->y = in->x * mat->m[0][1] + in->y * mat->m[1][1] + in->z * mat->m[2][1] + mat->m[3][1];
	out->z = in->x * mat->m[0][2] + in->y * mat->m[1][2] + in->z * mat->m[2][2] + mat->m[3][2];

	float w = in->x * mat->m[0][3] + in->y * mat->m[1][3] + in->z * mat->m[2][3] +mat->m[3][3];

	if (w != 0) {
		out->x /= w;
		out->y /= w;
		out->z /= w;
	}
}

