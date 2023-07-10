#ifndef _MESH_H
#define _MESH_H

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>
#include "3dmath.h"

typedef struct color {
	u_int8_t r;
	u_int8_t g;
	u_int8_t b;
} color_t;

typedef struct vec2d {
	float u;
	float v;
} vec2d_t;

typedef struct triangle {
	vec3d_t verts[3];
	vec2d_t texture[3];
	color_t color;
} triangle_t;

typedef struct mesh {
	triangle_t* triangles;
	int triangleCount;
	struct mesh* self;
} mesh_t;

mesh_t* CreateCube();

mesh_t* ReadMeshFromFile(char* file);

color_t createColor(u_int8_t r, u_int8_t g, u_int8_t b);

int compareZ(const void* e1, const void* e2);

int triangle_clipAgainstPlane(vec3d_t* planePoint, vec3d_t* planeNormal, triangle_t* triangleIn, triangle_t* triangleOut1, triangle_t* triangleOut2);
#endif