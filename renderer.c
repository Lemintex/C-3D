#include "renderer.h"
#include "matrix.h"
#include "mesh.h"
#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <stdio.h>

extern vec3d_t camera;

void DrawMesh(SDL_Renderer* renderer, mesh_t* mesh) {
	int trianglesToDraw = 0;
	triangle_t* sortedTriangles = (triangle_t*)malloc(0);

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

		//printf("TRI %d: x: %f, y: %f, z: %f\n", i, triangleTranslated.verts[0].x, triangleTranslated.verts[0].y, triangleTranslated.verts[0].z);
		//printf("TRI %d: x: %f, y: %f, z: %f\n", i, triangleTranslated.verts[1].x, triangleTranslated.verts[1].y, triangleTranslated.verts[1].z);
		//printf("TRI %d: x: %f, y: %f, z: %f\n", i, triangleTranslated.verts[2].x, triangleTranslated.verts[2].y, triangleTranslated.verts[2].z);
		triangleTranslated = triangleRotatedZX; 

		triangleTranslated.verts[0].z += 9.0;
		triangleTranslated.verts[1].z += 9.0;
		triangleTranslated.verts[2].z += 9.0;

		//	CALCULATE SHADING TO STORE IN TRIANGLE INFO
		vec3d_t normal, l1, l2;
		l1.x = triangleTranslated.verts[1].x - triangleTranslated.verts[0].x;
		l1.y = triangleTranslated.verts[1].y - triangleTranslated.verts[0].y;
		l1.z = triangleTranslated.verts[1].z - triangleTranslated.verts[0].z;

		l2.x = triangleTranslated.verts[2].x - triangleTranslated.verts[0].x;
		l2.y = triangleTranslated.verts[2].y - triangleTranslated.verts[0].y;
		l2.z = triangleTranslated.verts[2].z - triangleTranslated.verts[0].z;

		normal.x = l1.y * l2.z - l1.z * l2.y;
		normal.y = l1.z * l2.x - l1.x * l2.z;
		normal.z = l1.x * l2.y - l1.y * l2.x;

		float length = sqrt(pow(normal.x, 2) + pow(normal.y , 2) + pow(normal.z, 2));
		normal.x /= length; normal.y /= length; normal.z /= length;

		if (normal.x * (triangleTranslated.verts[0].x - camera.x) +
			normal.y * (triangleTranslated.verts[0].y - camera.y) +
			normal.z * (triangleTranslated.verts[0].z - camera.z) >= 0) continue;
		vec3d_t light_direction = {0, 0, -1};

		float l = sqrt(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
		light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;

		float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;

		//SDL_SetRenderDrawColor(renderer, dp * 255, dp * 255, dp * 255, SDL_ALPHA_OPAQUE);

		MultiplyMatrixByVector(triangleTranslated.verts[0], &(triangleProjected.verts[0]), *transformationMatrix);
		MultiplyMatrixByVector(triangleTranslated.verts[1], &(triangleProjected.verts[1]), *transformationMatrix);
		MultiplyMatrixByVector(triangleTranslated.verts[2], &(triangleProjected.verts[2]), *transformationMatrix);

		triangleProjected.col = dp * 255;
		printf("%d\n", triangleProjected.col);
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


		trianglesToDraw++;
		sortedTriangles = (triangle_t*)realloc(sortedTriangles, sizeof(triangle_t) * trianglesToDraw);
		sortedTriangles[trianglesToDraw - 1] = triangleProjected;
	}

	qsort(sortedTriangles, trianglesToDraw, sizeof(triangle_t), compareZ);

	for(int i = 0; i < trianglesToDraw; i++) {
		DrawTriangle(renderer, &sortedTriangles[i]);
	}

	free(transformationMatrix);
	delta += 0.001;
}

void DrawTriangle(SDL_Renderer* renderer, triangle_t* triangle) {
	
		vec3d_t normal, l1, l2;
		l1.x = triangle->verts[1].x - triangle->verts[0].x;
		l1.y = triangle->verts[1].y - triangle->verts[0].y;
		l1.z = triangle->verts[1].z - triangle->verts[0].z;

		l2.x = triangle->verts[2].x - triangle->verts[0].x;
		l2.y = triangle->verts[2].y - triangle->verts[0].y;
		l2.z = triangle->verts[2].z - triangle->verts[0].z;

		normal.x = l1.y * l2.z - l1.z * l2.y;
		normal.y = l1.z * l2.x - l1.x * l2.z;
		normal.z = l1.x * l2.y - l1.y * l2.x;

		float length = sqrt(pow(normal.x, 2) + pow(normal.y , 2) + pow(normal.z, 2));
		normal.x /= length; normal.y /= length; normal.z /= length;
		vec3d_t light_direction = {0, 0, -1};

		float l = sqrt(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
		light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;

		float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;


		SDL_SetRenderDrawColor(renderer, triangle->col, triangle->col,triangle->col, SDL_ALPHA_OPAQUE);
		//SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		FillTriangle(renderer, triangle);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		DrawWireframeTriangle(renderer, triangle);

}

void DrawWireframeTriangle(SDL_Renderer* renderer, triangle_t* triangle) {
	SDL_RenderDrawLine(renderer, triangle->verts[0].x, triangle->verts[0].y, triangle->verts[1].x, triangle->verts[1].y);
	SDL_RenderDrawLine(renderer, triangle->verts[1].x, triangle->verts[1].y, triangle->verts[2].x, triangle->verts[2].y);
	SDL_RenderDrawLine(renderer, triangle->verts[2].x, triangle->verts[2].y, triangle->verts[0].x, triangle->verts[0].y);

	//printf("%f, %f, %f, %f, %f, %f \n", triangle->verts[0].x, triangle->verts[0].y, triangle->verts[1].x, triangle->verts[1].y, triangle->verts[2].x, triangle->verts[2].y);
}

void FillTriangle(SDL_Renderer* renderer, triangle_t* triangle) {

	int maxY = 0;
	int minY = 0;
	for(int i = 1; i < 3; i++) {
		if(triangle->verts[i].y >= triangle->verts[maxY].y) maxY = i;
		else if(triangle->verts[i].y <= triangle->verts[minY].y) minY = i;
	}
	int midY = 3 - minY - maxY; // it works!

	printf("%d, %d\n", minY, maxY);

	vec2d_t vMax = {triangle->verts[maxY].x, triangle->verts[maxY].y};

	vec2d_t vMid = {triangle->verts[midY].x, triangle->verts[midY].y};

	vec2d_t vMin = {triangle->verts[minY].x, triangle->verts[minY].y};

	float slopeHypot = (vMax.x - vMin.x) / (vMax.y - vMin.y);
	float xHyp = vMin.x;

	FillTriangleTop(renderer, &vMin, &vMid, slopeHypot, &xHyp);

	FillTriangleBottom(renderer, &vMid, &vMax, slopeHypot, &xHyp);
}

void FillTriangleTop(SDL_Renderer* renderer, vec2d_t* vTop, vec2d_t* vMid, float slopeHyp, float* xHyp) {
	if(vMid->y - vTop->y < 1) return;

	float slopeA = (vTop->x - vMid->x) / (vTop->y - vMid->y);
	float x1 = vTop->x;
	float *a, *b;

	if(slopeA < slopeHyp) {
		a = &x1;
		b = xHyp;
	}
	else {
		a = xHyp;
		b = &x1;
	}

	for(int y = vTop->y; y<vMid->y; y++) {

		for(int x = *a; x < *b; x++) {
			SDL_RenderDrawPoint(renderer, x, y);
		}
		x1 += slopeA;
		*xHyp += slopeHyp;
	}
}

void FillTriangleBottom(SDL_Renderer* renderer, vec2d_t* vMid, vec2d_t* vBot, float slopeHyp, float* xHyp) {
	if(vBot->y - vMid->y < 1) return;

	float slopeB = (vMid->x - vBot->x) / (vMid->y - vBot->y);
	float x1 = vMid->x;
	float *a, *b;

	if(slopeB > slopeHyp) {
	   	a = &x1;
	   	b = xHyp;
	}
	else {
		a = xHyp;
   		b = &x1;
	}

	for(int y = vMid->y; y<vBot->y; y++) {

		for(int x = *a; x < *b; x++ ) {
			SDL_RenderDrawPoint(renderer, x, y);
		}	
		x1 += slopeB;
		*xHyp += slopeHyp;
	}
}

