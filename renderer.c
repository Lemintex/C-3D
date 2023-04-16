#include "renderer.h"
#include "matrix.h"
#include "mesh.h"
#include <stdlib.h>
#include <stdio.h>

void DrawMesh(SDL_Renderer* renderer, mesh_t* mesh) {
	static float delta = 0;
	matrix_4x4_t matRotZ = {0};
	matrix_4x4_t matRotX = {0};

	matRotZ.m[0][0] = cosf(delta);
	matRotZ.m[0][1] = sinf(delta);
	matRotZ.m[1][0] = -sinf(delta);
	matRotZ.m[1][1] = cosf(delta);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(delta * 0.5f);
	matRotX.m[1][2] = sinf(delta * 0.5f);
	matRotX.m[2][1] = -sinf(delta * 0.5f);
	matRotX.m[2][2] = cosf(delta * 0.5f);
	matRotX.m[3][3] = 1;

	matrix_4x4_t* transformationMatrix = GetTranformationMatrix(500, 500);
	for (int i = 0; i < mesh->triangleCount; i++) {
		triangle_t triangle;
		triangle = mesh->triangles[i];

	//	printf("%f, %f, %f\n", triangle.verts[0].x,triangle.verts[0].y,triangle.verts[0].z);
	//	printf("%f, %f, %f\n", triangle.verts[1].x,triangle.verts[1].y,triangle.verts[1].z);
	//	printf("%f, %f, %f\n\n", triangle.verts[2].x,triangle.verts[2].y,triangle.verts[2].z);

		triangle_t triangleProjected;
		triangle_t triangleTranslated;

		triangle_t triangleRotatedZ;
		MultiplyMatrixByVector(triangle.verts[0], &(triangleRotatedZ.verts[0]), matRotZ);
		MultiplyMatrixByVector(triangle.verts[1], &(triangleRotatedZ.verts[1]), matRotZ);
		MultiplyMatrixByVector(triangle.verts[2], &(triangleRotatedZ.verts[2]), matRotZ);

		triangle_t triangleRotatedZX;
		MultiplyMatrixByVector(triangleRotatedZ.verts[0], &(triangleRotatedZX.verts[0]), matRotX);
		MultiplyMatrixByVector(triangleRotatedZ.verts[1], &(triangleRotatedZX.verts[1]), matRotX);
		MultiplyMatrixByVector(triangleRotatedZ.verts[2], &(triangleRotatedZX.verts[2]), matRotX);

//		triangleTranslated = triangle; 

		printf("TRI %d: x: %f, y: %f, z: %f\n", i, triangleTranslated.verts[0].x, triangleTranslated.verts[0].y, triangleTranslated.verts[0].z);
		printf("TRI %d: x: %f, y: %f, z: %f\n", i, triangleTranslated.verts[1].x, triangleTranslated.verts[1].y, triangleTranslated.verts[1].z);
		printf("TRI %d: x: %f, y: %f, z: %f\n", i, triangleTranslated.verts[2].x, triangleTranslated.verts[2].y, triangleTranslated.verts[2].z);
		triangleTranslated = triangleRotatedZX; 

		triangleTranslated.verts[0].z += 3.0;
		triangleTranslated.verts[1].z += 3.0;
		triangleTranslated.verts[2].z += 3.0;

		MultiplyMatrixByVector(triangleTranslated.verts[0], &(triangleProjected.verts[0]), *transformationMatrix);
		MultiplyMatrixByVector(triangleTranslated.verts[1], &(triangleProjected.verts[1]), *transformationMatrix);
		MultiplyMatrixByVector(triangleTranslated.verts[2], &(triangleProjected.verts[2]), *transformationMatrix);

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
	delta += 0.0001;
}

void DrawTriangle(SDL_Renderer* renderer, triangle_t* triangle) {
	SDL_RenderDrawLine(renderer, triangle->verts[0].x, triangle->verts[0].y, triangle->verts[1].x, triangle->verts[1].y);
	SDL_RenderDrawLine(renderer, triangle->verts[1].x, triangle->verts[1].y, triangle->verts[2].x, triangle->verts[2].y);
	SDL_RenderDrawLine(renderer, triangle->verts[2].x, triangle->verts[2].y, triangle->verts[0].x, triangle->verts[0].y);

	printf("%f, %f, %f, %f, %f, %f \n", triangle->verts[0].x, triangle->verts[0].y, triangle->verts[1].x, triangle->verts[1].y, triangle->verts[2].x, triangle->verts[2].y);
}
