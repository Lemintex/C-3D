#ifndef _MESH_H
#define _MESH_H

#include <stdlib.h>
typedef struct vec3d {
	float x, y, z;
} vec3d_t;

typedef struct triangle {
	vec3d_t verts[3];
} triangle_t;

typedef struct mesh {
	struct triangle* triangles;
	int triangleCount;
	struct mesh* self;
} mesh_t;

mesh_t* CreateCube();

triangle_t CreateTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3); 

#endif
