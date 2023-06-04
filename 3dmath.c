#include "3dmath.h"

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

vec3d_t vec3_intersectPlane(vec3d_t* planePoint, vec3d_t* planeNormal, vec3d_t* lineStart, vec3d_t* lineEnd) {
	*planeNormal = vec3_normal(planeNormal);
	vec3d_t dir = vec3_sub(lineEnd, lineStart);
	float dot = vec3_dot(&dir, planeNormal);

    float t = ((planePoint->x - lineStart->x) * planeNormal->x +
                   (planePoint->y - lineStart->y) * planeNormal->y +
                   (planePoint->z - lineStart->z) * planeNormal->z) / dot;
        
	vec3d_t distance = vec3_mul(&dir, t);
	return vec3_add(&distance, lineStart);
//	float planeDot = -vec3_dot(planeNormal, planePoint);
//	float distanceStart = vec3_dot(lineStart, planeNormal);
//	float distanceEnd = vec3_dot(lineEnd, planeNormal);
//	float t = (-planeDot - distanceStart) / (distanceEnd - distanceStart);
//	vec3d_t startToEnd = vec3_sub(lineEnd, lineStart);
//	vec3d_t lineToIntersect = vec3_mul(&startToEnd, t);
//	return vec3_add(&lineToIntersect, lineStart);
}

matrix_4x4_t matrix_identity() {
	matrix_4x4_t matrix = {0};
	matrix.m[0][0] = 1;
	matrix.m[1][1] = 1;
	matrix.m[2][2] = 1;
	matrix.m[3][3] = 1;
	return matrix;
}

matrix_4x4_t matrix_rotationX(float angleRad) {
	matrix_4x4_t matrix = {0};
	matrix.m[0][0] = 1;
	matrix.m[1][1] = cosf(angleRad);
	matrix.m[1][2] = sinf(angleRad);
	matrix.m[2][1] = -sinf(angleRad);
	matrix.m[2][2] = cosf(angleRad);
	matrix.m[3][3] = 1;
	return matrix;
}

matrix_4x4_t matrix_rotationY(float angleRad) {
	matrix_4x4_t matrix = {0};
	matrix.m[0][0] = cosf(angleRad);
	matrix.m[0][2] = sinf(angleRad);
	matrix.m[1][1] = 1;
	matrix.m[2][0] = -sinf(angleRad);
	matrix.m[2][2] = cosf(angleRad);
	matrix.m[3][3] = 1;
	return matrix;
}

matrix_4x4_t matrix_rotationZ(float angleRad) {
	matrix_4x4_t matrix = {0};
	matrix.m[0][0] = cosf(angleRad);
	matrix.m[0][1] = sinf(angleRad);
	matrix.m[1][0] = -sinf(angleRad);
	matrix.m[1][1] = cosf(angleRad);
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

matrix_4x4_t matrix_projection(float fFovDegrees, float fAspectRatio, float fNear, float fFar) {
	float fFovRad = 1 / tanf((fFovDegrees / 2) / 180 * M_PI);
	matrix_4x4_t matrix = {0};
	matrix.m[0][0] = fAspectRatio * fFovRad;
	matrix.m[1][1] = fFovRad;
	matrix.m[2][2] = fFar / (fFar - fNear);
	matrix.m[2][3] = 1.0;
	matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matrix.m[3][3] = 0.0;
	return matrix;
}

matrix_4x4_t matrix_multiplyMatrix(matrix_4x4_t* m1, matrix_4x4_t* m2) {
	matrix_4x4_t matrix = {0};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
		    matrix.m[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                matrix.m[i][j] += m1->m[i][k] * m2->m[k][j];
            }
		}
	}
	return matrix;
}

matrix_4x4_t matrix_pointAt(vec3d_t* pos, vec3d_t* target, vec3d_t* up) {
	vec3d_t newForward = vec3_sub(pos, target);
	newForward = vec3_normal(&newForward);

	vec3d_t a = vec3_mul(&newForward, vec3_dot(up, &newForward));
	vec3d_t newUp = vec3_sub(up, &a);
	newUp = vec3_normal(&newUp);

	vec3d_t newRight = vec3_cross(&newUp, &newForward);

	matrix_4x4_t matrix;
	matrix.m[0][0] = newRight.x;
	matrix.m[0][1] = newRight.y;
	matrix.m[0][2] = newRight.z;
	matrix.m[0][3] = 0;

	matrix.m[1][0] = newUp.x;
	matrix.m[1][1] = newUp.y;
	matrix.m[1][2] = newUp.z;
	matrix.m[1][3] = 0;

	matrix.m[2][0] = newForward.x;
	matrix.m[2][1] = newForward.y;
	matrix.m[2][2] = newForward.z;
	matrix.m[2][3] = 0;

	matrix.m[3][0] = pos->x;
	matrix.m[3][1] = pos->y;
	matrix.m[3][2] = pos->z;
	matrix.m[3][3] = 1;
	return matrix;
}

	matrix_4x4_t matrix_quickInverse(matrix_4x4_t* mat) // Only for Rotation/Translation Matrices
	{
		matrix_4x4_t m = *mat;
		matrix_4x4_t matrix;
		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}
