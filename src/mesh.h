#ifndef _MESH_H
#define _MESH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>
#include "3dmath.h"

typedef struct mesh {
	triangle_t *triangles;
	int triangle_count;
	struct mesh *self;
} mesh_t;

mesh_t* create_cube();

mesh_t* read_mesh_from_file(char *file, int has_texture);

color_t create_color(unsigned char r, unsigned char g, unsigned char b);

// REMOVE
int compareZ(const void *e1, const void *e2);

int triangle_clip_against_plane(vec3d_t *plane_point, vec3d_t *plane_normal, triangle_t *triangle_in, triangle_t *triangle_out_1, triangle_t *triangle_out_2);

#endif
