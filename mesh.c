#include "mesh.h"

mesh_t* CreateCube() {
	mesh_t* cube = (mesh_t*)malloc(sizeof(mesh_t));
	cube->self = cube;
	cube->triangles = (triangle_t*)malloc(sizeof(triangle_t) * 12);
	cube->triangleCount = 12;
		// SOUTH
	cube->triangles[0] = (triangle_t){ 0.0, 0.0, 0.0,    0.0, 1.0, 0.0,    1.0, 1.0, 0.0 };
	cube->triangles[1] = (triangle_t){ 0.0, 0.0, 0.0,    1.0, 1.0, 0.0,    1.0, 0.0, 0.0 };

		// EAST                                                      
	cube->triangles[2] = (triangle_t){ 1.0, 0.0, 0.0,    1.0, 1.0, 0.0,    1.0, 1.0, 1.0 };
	cube->triangles[3] = (triangle_t){ 1.0, 0.0, 0.0,    1.0, 1.0, 1.0,    1.0, 0.0, 1.0 };

		// NORTH                                                    
	cube->triangles[4] = (triangle_t){ 1.0, 0.0, 1.0,    1.0, 1.0, 1.0,    0.0, 1.0, 1.0 };
	cube->triangles[5] = (triangle_t){ 1.0, 0.0, 1.0,    0.0, 1.0, 1.0,    0.0, 0.0, 1.0 };

		// WEST                                                     
	cube->triangles[6] = (triangle_t){ 0.0, 0.0, 1.0,    0.0, 1.0, 1.0,    0.0, 1.0, 0.0 };
	cube->triangles[7] = (triangle_t){ 0.0, 0.0, 1.0,    0.0, 1.0, 0.0,    0.0, 0.0, 0.0 };

		// TOP                                                       
	cube->triangles[8] = (triangle_t){ 0.0, 1.0, 0.0,    0.0, 1.0, 1.0,    1.0, 1.0, 1.0 };
	cube->triangles[9] = (triangle_t){ 0.0, 1.0, 0.0,    1.0, 1.0, 1.0,    1.0, 1.0, 0.0 };

		// BOTTOM                                                    
	cube->triangles[10] = (triangle_t){ 1.0, 0.0, 1.0,    0.0, 0.0, 1.0,    0.0, 0.0, 0.0 };
	cube->triangles[11] = (triangle_t){ 1.0, 0.0, 1.0,    0.0, 0.0, 0.0,    1.0, 0.0, 0.0 };

	return cube;
}
