#ifndef _MESH_H
#define _MESH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>
#include "3dmath.h"

typedef struct triangle {
	vec3d_t verts[3];
	char col;
} triangle_t;

typedef struct mesh {
	triangle_t* triangles;
	int triangleCount;
	struct mesh* self;
} mesh_t;

mesh_t* CreateCube();

mesh_t* ReadMeshFromFile(char* file);

int compareZ(const void* e1, const void* e2);
#endif
