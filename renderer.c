#include "renderer.h"
#include "matrix.h"
#include "mesh.h"
#include <stdlib.h>
#include <stdio.h>

void DrawMesh(SDL_Renderer* renderer, mesh_t* mesh) {
	
	matrix_4x4_t* transformationMatrix = GetTranformationMatrix(500, 500);
	for (int i = 0; i < mesh->triangleCount; i++) {
		triangle_t triangle;
		triangle = mesh->triangles[i];

		triangle_t triangleProjected;
		triangle_t triangleTranslated;

		triangleTranslated = mesh->triangles[i];

		triangleTranslated.verts[0].z = triangle.verts[0].z + 3.0;
		triangleTranslated.verts[1].z = triangle.verts[1].z + 3.0;
		triangleTranslated.verts[2].z = triangle.verts[2].z + 3.0;

		MultiplyMatrixByVector(triangleTranslated.verts[0], &(triangleProjected.verts[0]), transformationMatrix);
		MultiplyMatrixByVector(triangleTranslated.verts[1], &(triangleProjected.verts[1]), transformationMatrix);
		MultiplyMatrixByVector(triangleTranslated.verts[2], &(triangleProjected.verts[2]), transformationMatrix);

		triangleProjected.verts[0].x += 1;
		triangleProjected.verts[0].y += 1;

		triangleProjected.verts[1].x += 1;
		triangleProjected.verts[1].y += 1;

		triangleProjected.verts[2].x += 1;
		triangleProjected.verts[2].y += 1;

		triangleProjected.verts[0].x *= 0.5 * 500;
		triangleProjected.verts[0].y *= 0.5 * 500;

		triangleProjected.verts[1].x *= 0.5 * 500;
		triangleProjected.verts[1].y *= 0.5 * 500;

		triangleProjected.verts[2].x *= 0.5 * 500;
		triangleProjected.verts[2].y *= 0.5 * 500;


		DrawTriangle(renderer, &triangleProjected);
	}
	free(transformationMatrix);
}

void DrawTriangle(SDL_Renderer* renderer, triangle_t* triangle) {
	SDL_RenderDrawLine(renderer, triangle->verts[0].x, triangle->verts[0].y, triangle->verts[1].x, triangle->verts[1].y);
	SDL_RenderDrawLine(renderer, triangle->verts[1].x, triangle->verts[1].y, triangle->verts[2].x, triangle->verts[2].y);
	SDL_RenderDrawLine(renderer, triangle->verts[2].x, triangle->verts[2].y, triangle->verts[0].x, triangle->verts[0].y);
	printf("%f, %f, %f, %f \n", triangle->verts[0].x, triangle->verts[0].y, triangle->verts[1].x, triangle->verts[1].y);
}
