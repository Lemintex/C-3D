#include "mesh.h"

mesh_t* CreateCube() {
	mesh_t* cube = (mesh_t*)malloc(sizeof(mesh_t));
	cube->self = cube;
	cube->triangles = (triangle_t*)malloc(sizeof(triangle_t) * 12);
	cube->triangleCount = 12;

	// SOUTH
	*(cube->triangles+0) = CreateTriangle(0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0);
	*(cube->triangles+1) = CreateTriangle(0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0);

	// EAST
	*(cube->triangles+2) = CreateTriangle(1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0);
	*(cube->triangles+3) = CreateTriangle(1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0);

	// NORTH
	*(cube->triangles+4) = CreateTriangle(1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0);
	*(cube->triangles+5) = CreateTriangle(1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0);

	// WEST
	*(cube->triangles+6) = CreateTriangle(0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0);
	*(cube->triangles+7) = CreateTriangle(0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0);

	// TOP
	*(cube->triangles+8) = CreateTriangle(0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0);
	*(cube->triangles+9) = CreateTriangle(0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0);

	// BOTTOM
	*(cube->triangles+10) = CreateTriangle(1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0);
	*(cube->triangles+11) = CreateTriangle(1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
	return cube;
}
triangle_t CreateTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {

	triangle_t* triangle = (triangle_t*)malloc(sizeof(triangle_t));

	triangle->verts[0].x = x1;
	triangle->verts[0].y = y1;
	triangle->verts[0].z = z1;

	triangle->verts[1].x = x2;
	triangle->verts[1].y = y2;
	triangle->verts[1].z = z2;

	triangle->verts[2].x = x3;
	triangle->verts[2].y = y3;
	triangle->verts[2].z = z3;

	return *triangle;
}
