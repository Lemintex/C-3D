#ifndef _VEC_H_
#define _VEC_H_

typedef struct {
	float x;
	float y;
	float z;
} vec3d_t;

typedef struct {
	float x;
	float y;
} vec2d_t;

void normalise(vec3d_t* v);

#endif
