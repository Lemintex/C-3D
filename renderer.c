#include "renderer.h"
#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

void DrawMesh(SDL_Renderer* renderer, mesh_t* mesh) {
	matrix_4x4_t* transformationMatrix = GetTranformationMatrix(500, 500);
	for (int i = 0; i < mesh->triangleCount; i++) {
		triangle_t triangleProjected;

		MultiplyMatrixByVector(&(mesh->triangles+i)->verts[0], &(triangleProjected.verts[0]), transformationMatrix);
		MultiplyMatrixByVector(&(mesh->triangles+i)->verts[1], &(triangleProjected.verts[1]), transformationMatrix);
		MultiplyMatrixByVector(&(mesh->triangles+i)->verts[2], &(triangleProjected.verts[2]), transformationMatrix);
		DrawTriangle(renderer, &triangleProjected);
	}
	free(transformationMatrix);
}

void DrawTriangle(SDL_Renderer* renderer, triangle_t* triangle) {
	SDL_RenderDrawLine(renderer, triangle->verts[0].x, triangle->verts[0].y, triangle->verts[1].x, triangle->verts[1].y);
	SDL_RenderDrawLine(renderer, triangle->verts[1].x, triangle->verts[1].y, triangle->verts[2].x, triangle->verts[2].y);
	SDL_RenderDrawLine(renderer, triangle->verts[2].x, triangle->verts[2].y, triangle->verts[0].x, triangle->verts[0].y);
//	printf("%f, %f, %f, %f \n", triangle->verts[0].x, triangle->verts[0].y, triangle->verts[1].x, triangle->verts[1].y);
}
