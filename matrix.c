#include "matrix.h"

matrix_4x4_t matrix_identity() {
	matrix_4x4_t matrix;
	matrix.m[0][0] = 1;
	matrix.m[1][1] = 1;
	matrix.m[2][2] = 1;
	matrix.m[3][3] = 1;
	return matrix;
}

matrix_4x4_t matrix_rotationX(float angleRad) {
	matrix_4x4_t matrix;
	matrix.m[0][0] = 1;
	matrix.m[1][1] = cosf(angleRad);
	matrix.m[1][2] = sinf(angleRad);
	matrix.m[2][1] = -sinf(angleRad);
	matrix.m[2][2] = cosf(angleRad);
	matrix.m[3][3] = 1;
}

matrix_4x4_t matrix_rotationY(float angleRad) {
	matrix_4x4_t matrix;
	matrix.m[0][0] = cosf(angleRad);
	matrix.m[0][2] = sinf(angleRad);
	matrix.m[1][1] = 1;
	matrix.m[2][0] = -sinf(angleRad);
	matrix.m[2][2] = cosf(angleRad);
	matrix.m[3][3] = 1;
	return matrix;
}

matrix_4x4_t matrix_rotationZ(float angleRad) {
	matrix_4x4_t matrix;
	matrix.m[0][0] = cosf(angleRad);
	matrix.m[0][1] = sinf(angleRad);
	matrix.m[1][0] = -sinf(angleRad);
	matrix.m[1][1] = cosf(angleRad);
	matrix.m[2][2] = 1;
	matrix.m[3][3] = 1;
	return matrix;
}

matrix_4x4_t matrix_translation(float x, float y, float z) {
	matrix_4x4_t matrix;
	matrix.m[0][0] = 1;
	matrix.m[1][1] = 1;
	matrix.m[2][2] = 1;
	matrix.m[3][3] = 1;

	matrix.m[3][0] = x;
	matrix.m[3][1] = y;
	matrix.m[3][1] = z;
	return  matrix;
}

matrix_4x4_t matrix_projection(float fFovDegrees, float fAspectRatio, float fNear, float fFar) {
	float fFovRad = 1 / tanf(fFovDegrees * 0.5 / 180 * M_PI);
	matrix_4x4_t matrix;
	matrix.m[0][0] = fAspectRatio * fFovRad;
	matrix.m[1][1] = fFovRad;
	matrix.m[2][2] = fFar / (fFar - fNear);
	matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][3] = 0.0f;
	return matrix;
}

matrix_4x4_t matrix_multiplyMatrix(matrix_4x4_t* m1, matrix_4x4_t* m2) {
	matrix_4x4_t matrix;
	for (int c = 0; c < 4; c++) {
		for (int r = 0; r < 4; r++) {
			matrix.m[r][c] = m1->m[r][0] * m2->m[0][c] + m1->m[r][1] * m2->m[1][c] + m1->m[r][2] * m2->m[2][c] + m1->m[r][3] * m2->m[3][c];
		}
	}
	return matrix;
}

matrix_4x4_t* CreateEmptyMatrix() {
	matrix_4x4_t* matrix = (matrix_4x4_t*)calloc(1, sizeof(matrix_4x4_t));
	return matrix;
}
