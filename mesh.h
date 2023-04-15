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
	triangle_t* triangles;
	int triangleCount;
	struct mesh* self;
} mesh_t;

mesh_t* CreateCube();

#endif
