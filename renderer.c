#include "renderer.h"
#include "matrix.h"
#include "mesh.h"
#include "vec.h"
#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <stdio.h>

extern vec3d_t camera;

void DrawMesh(SDL_Renderer* renderer, mesh_t* mesh) {
	int trianglesToDraw = 0;
	triangle_t* sortedTriangles = (triangle_t*)malloc(0);

	matrix_4x4_t matProj = matrix_projection(90, 1 /*w/h*/, 0.1, 1000);

	static float delta = 0;
	matrix_4x4_t matRotZ = matrix_rotationZ(delta);
	matrix_4x4_t matRotX = matrix_rotationX(delta);
	matrix_4x4_t matRotA;
	matrix_4x4_t matTrans = matrix_translation(0, 0, 10);

	matrix_4x4_t matWorld = matrix_identity();
	matRotA = matrix_multiplyMatrix(&matRotZ, &matRotX);

	matWorld = matrix_multiplyMatrix(&matWorld, &matRotA);
	matWorld = matrix_multiplyMatrix(&matWorld, &matTrans);

	for (int i = 0; i < mesh->triangleCount; i++) {
		triangle_t triangle = mesh->triangles[i];

	printf("Triangle; %f, %f, %f, %f, %f, %f \n", triangle.verts[0].x, triangle.verts[0].y, triangle.verts[1].x, triangle.verts[1].y, triangle.verts[2].x, triangle.verts[2].y);
	//	printf("%f, %f, %f\n", triangle.verts[0].x,triangle.verts[0].y,triangle.verts[0].z);
	//	printf("%f, %f, %f\n", triangle.verts[1].x,triangle.verts[1].y,triangle.verts[1].z);
	//	printf("%f, %f, %f\n\n", triangle.verts[2].x,triangle.verts[2].y,triangle.verts[2].z);

		triangle_t triangleProjected;
		triangle_t triangleTransformed;

		printf("World: %f, %f, %f\n", triangle.verts[0].x, triangle.verts[0].y, triangle.verts[0].z);

//		triangleTranslated = triangle;
		triangleTransformed.verts[0] = vec3_mul_mat4(&triangle.verts[0], &matWorld);
		triangleTransformed.verts[1] = vec3_mul_mat4(&triangle.verts[1], &matWorld);
		triangleTransformed.verts[2] = vec3_mul_mat4(&triangle.verts[2], &matWorld);
		printf("World: %f, %f, %f\n", triangleTransformed.verts[0].x, triangleTransformed.verts[0].y, triangleTransformed.verts[0].z);

		triangleTransformed.verts[0].z += 10; // = vec3_mul_mat4(&triangle.verts[0], &matWorld);
		triangleTransformed.verts[1].z += 10; // = vec3_mul_mat4(&triangle.verts[1], &matWorld);
		triangleTransformed.verts[2].z += 10; // = vec3_mul_mat4(&triangle.verts[2], &matWorld);
		//printf("TRI %d: x: %f, y: %f, z: %f\n", i, triangleTranslated.verts[0].x, triangleTranslated.verts[0].y, triangleTranslated.verts[0].z);
		//printf("TRI %d: x: %f, y: %f, z: %f\n", i, triangleTranslated.verts[1].x, triangleTranslated.verts[1].y, triangleTranslated.verts[1].z);
		//printf("TRI %d: x: %f, y: %f, z: %f\n", i, triangleTranslated.verts[2].x, triangleTranslated.verts[2].y, triangleTranslated.verts[2].z);

		//	CALCULATE SHADING TO STORE IN TRIANGLE INFO

		vec3d_t normal, l1, l2;

		l1 = vec3_sub(&triangleTransformed.verts[1], &triangleTransformed.verts[0]);
		l2 = vec3_sub(&triangleTransformed.verts[2], &triangleTransformed.verts[0]);

		normal = vec3_cross(&l1, &l2);

		normal = vec3_normal(&normal);
		
		vec3d_t rayFromCamera = vec3_sub(&triangleTransformed.verts[0], &camera);

//		if (vec3_dot(&normal, &rayFromCamera) >= 0) continue;
		vec3d_t light_direction = {0, 0, -1};
		light_direction =vec3_normal(&light_direction);

		float dp = vec3_dot(&light_direction, &normal);

		//SDL_SetRenderDrawColor(renderer, dp * 255, dp * 255, dp * 255, SDL_ALPHA_OPAQUE);

		triangleProjected.verts[0] = vec3_mul_mat4(&triangleTransformed.verts[0], &matProj);
		triangleProjected.verts[1] = vec3_mul_mat4(&triangleTransformed.verts[1], &matProj);
		triangleProjected.verts[2] = vec3_mul_mat4(&triangleTransformed.verts[2], &matProj);

		// normalise co-ordinates
		triangleProjected.verts[0] = vec3_div(&triangleProjected.verts[0], triangleProjected.verts[0].w);
		triangleProjected.verts[1] = vec3_div(&triangleProjected.verts[1], triangleProjected.verts[1].w);
		triangleProjected.verts[2] = vec3_div(&triangleProjected.verts[2], triangleProjected.verts[2].w);

		triangleProjected.col = dp * 255;
		printf("%d\n", triangleProjected.col);

		// offset into view
		vec3d_t vOffsetView = (vec3d_t){1, 1, 0};

		triangleProjected.verts[0] = vec3_add(&triangleProjected.verts[0], &vOffsetView);
		triangleProjected.verts[1] = vec3_add(&triangleProjected.verts[1], &vOffsetView);
		triangleProjected.verts[2] = vec3_add(&triangleProjected.verts[2], &vOffsetView);

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

	printf("%f, %f, %f, %f, %f, %f \n", triangle->verts[0].x, triangle->verts[0].y, triangle->verts[1].x, triangle->verts[1].y, triangle->verts[2].x, triangle->verts[2].y);
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

