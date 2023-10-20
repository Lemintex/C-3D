#include "renderer.h"

#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>

#include "3dmath.h"
#include "queue.h"

extern camera_t camera;
extern int width, height;
extern float *depthBuffer;

void DrawMesh(SDL_Renderer *renderer, mesh_t *mesh, SDL_Surface *texture)
{
	int trianglesToDraw = 0;
	triangle_t *sortedTriangles = (triangle_t *)malloc(0);

	matrix_4x4_t matProj = matrix_projection(90, 1 /*w/h*/, 0.1, 1000);

	static float delta = 0;
	matrix_4x4_t matRotZ = matrix_rotationZ(delta);
	matrix_4x4_t matRotX = matrix_rotationX(delta);
	matrix_4x4_t matRotA;
	matrix_4x4_t matTrans = matrix_translation(5, 0, 10);

	matrix_4x4_t matWorld = matrix_identity();
	matRotA = matrix_multiplyMatrix(&matRotZ, &matRotX);

	matWorld = matrix_multiplyMatrix(&matWorld, &matTrans);

	vec3d_t up = (vec3d_t){0, 1, 0, 1};
	vec3d_t target = (vec3d_t){0, 0, 1, 1};
	matrix_4x4_t yaw = matrix_rotationY(camera.yaw);

	camera.lookDir = vec3_mul_mat4(&target, &yaw);
	target = vec3_add(&camera.pos, &camera.lookDir);

	matrix_4x4_t cameraMatrix = matrix_pointAt(&camera.pos, &target, &up);

	matrix_4x4_t cameraView = matrix_quickInverse(&cameraMatrix);

	for (int i = 0; i < mesh->triangleCount; i++)
	{
		triangle_t triangle = mesh->triangles[i];

		triangle_t triangleProjected;
		triangle_t triangleTransformed;
		triangle_t triangleViewed;

		triangleTransformed.verts[0] = vec3_mul_mat4(&triangle.verts[0], &matWorld);
		triangleTransformed.verts[1] = vec3_mul_mat4(&triangle.verts[1], &matWorld);
		triangleTransformed.verts[2] = vec3_mul_mat4(&triangle.verts[2], &matWorld);

		triangleTransformed.texture[0] = triangle.texture[0];
		triangleTransformed.texture[1] = triangle.texture[1];
		triangleTransformed.texture[2] = triangle.texture[2];
		//	CALCULATE SHADING TO STORE IN TRIANGLE INFO

		vec3d_t normal, l1, l2;

		l1 = vec3_sub(&triangleTransformed.verts[1], &triangleTransformed.verts[0]);
		l2 = vec3_sub(&triangleTransformed.verts[2], &triangleTransformed.verts[0]);

		normal = vec3_cross(&l1, &l2);

		normal = vec3_normal(&normal);

		vec3d_t rayFromCamera = vec3_sub(&triangleTransformed.verts[1], &camera.pos);

		if (vec3_dot(&normal, &rayFromCamera) >= 0)
			continue;
		vec3d_t light_direction = {0, 0, -1};
		light_direction = vec3_normal(&light_direction);

		float dp = vec3_dot(&light_direction, &normal);

		triangleViewed.verts[0] = vec3_mul_mat4(&triangleTransformed.verts[0], &cameraView);
		triangleViewed.verts[1] = vec3_mul_mat4(&triangleTransformed.verts[1], &cameraView);
		triangleViewed.verts[2] = vec3_mul_mat4(&triangleTransformed.verts[2], &cameraView);

		triangleViewed.texture[0] = triangleTransformed.texture[0];
		triangleViewed.texture[1] = triangleTransformed.texture[1];
		triangleViewed.texture[2] = triangleTransformed.texture[2];
		// clip against near plane of camera
		int clippedTriangles = 0;
		triangle_t clipped[2];

		vec3d_t nearPlane = (vec3d_t){0, 0, 0.1, 1};
		vec3d_t nearPlaneNormal = (vec3d_t){0, 0, 1, 1}; // FRONT PLANE

		clippedTriangles = triangle_clipAgainstPlane(&nearPlane, &nearPlaneNormal, &triangleViewed,
													 &clipped[0], &clipped[1]);

		for (int j = 0; j < clippedTriangles; j++)
		{
			triangleProjected.verts[0] = vec3_mul_mat4(&clipped[j].verts[0], &matProj);
			triangleProjected.verts[1] = vec3_mul_mat4(&clipped[j].verts[1], &matProj);
			triangleProjected.verts[2] = vec3_mul_mat4(&clipped[j].verts[2], &matProj);

			triangleProjected.texture[0] = clipped[j].texture[0];
			triangleProjected.texture[1] = clipped[j].texture[1];
			triangleProjected.texture[2] = clipped[j].texture[2];

			triangleProjected.texture[0].u /= triangleProjected.verts[0].w;
			triangleProjected.texture[1].u /= triangleProjected.verts[1].w;
			triangleProjected.texture[2].u /= triangleProjected.verts[2].w;

			triangleProjected.texture[0].v /= triangleProjected.verts[0].w;
			triangleProjected.texture[1].v /= triangleProjected.verts[1].w;
			triangleProjected.texture[2].v /= triangleProjected.verts[2].w;

			triangleProjected.texture[0].w = 1.0 / triangleProjected.verts[0].w;
			triangleProjected.texture[1].w = 1.0 / triangleProjected.verts[1].w;
			triangleProjected.texture[2].w = 1.0 / triangleProjected.verts[2].w;

			// normalise co-ordinates
			triangleProjected.verts[0] = vec3_div(&triangleProjected.verts[0], triangleProjected.verts[0].w);
			triangleProjected.verts[1] = vec3_div(&triangleProjected.verts[1], triangleProjected.verts[1].w);
			triangleProjected.verts[2] = vec3_div(&triangleProjected.verts[2], triangleProjected.verts[2].w);

			uint8_t shade = dp * 128 + 127;
			triangleProjected.color = createColor(shade, shade, shade);

			// offset into view
			vec3d_t vOffsetView = (vec3d_t){1, 1, 0};

			triangleProjected.verts[0] = vec3_add(&triangleProjected.verts[0], &vOffsetView);
			triangleProjected.verts[1] = vec3_add(&triangleProjected.verts[1], &vOffsetView);
			triangleProjected.verts[2] = vec3_add(&triangleProjected.verts[2], &vOffsetView);

			triangleProjected.verts[0].x *= 0.5 * width;
			triangleProjected.verts[0].y *= 0.5 * height;

			triangleProjected.verts[1].x *= 0.5 * width;
			triangleProjected.verts[1].y *= 0.5 * height;

			triangleProjected.verts[2].x *= 0.5 * width;
			triangleProjected.verts[2].y *= 0.5 * height;

			trianglesToDraw++;
			sortedTriangles = (triangle_t *)realloc(
				sortedTriangles, sizeof(triangle_t) * trianglesToDraw);
			sortedTriangles[trianglesToDraw - 1] = triangleProjected;
		}
	}

	qsort(sortedTriangles, trianglesToDraw, sizeof(triangle_t), compareZ);

	queue_t *clippedTrianglesToDraw = createQueue();

	for (int i = 0; i < trianglesToDraw; i++)
	{
		triangle_t clipped[2];

		queue_t *queue = createQueue();
		enqueue(queue, sortedTriangles[i]);

		int nNewTriangles = 1;

		for (int p = 0; p < 4; p++)
		{
			int trianglesToAdd = 1;
			while (nNewTriangles > 0)
			{
				nNewTriangles--;
				if (isEmpty(queue))
					continue;
				triangle_t test = dequeue(queue);

				switch (p)
				{
				case 0:
					trianglesToAdd = triangle_clipAgainstPlane(
						&((vec3d_t){0, 0, 0, 1}), &((vec3d_t){0, -1, 0, 1}), &test,
						&clipped[0], &clipped[1]); // TOP
					break;

				case 1:
					trianglesToAdd = triangle_clipAgainstPlane(
						&((vec3d_t){0, -width, 0, 1}), &((vec3d_t){0, 1, 0, 1}), &test,
						&clipped[0], &clipped[1]); // BOTTOM
					break;

				case 2:
					trianglesToAdd = triangle_clipAgainstPlane(
						&((vec3d_t){0, 0, 0, 1}), &((vec3d_t){-1, 0, 0, 1}), &test,
						&clipped[0], &clipped[1]); // LEFT
					break;

				case 3:
					trianglesToAdd = triangle_clipAgainstPlane(
						&((vec3d_t){-height, 0, 0, 1}), &((vec3d_t){1, 0, 0, 1}), &test,
						&clipped[0], &clipped[1]); // RIGHT
					break;

				default:
					break;
				}

				for (int t = 0; t < trianglesToAdd; t++)
				{
					enqueue(queue, clipped[t]);
				}
			}
			nNewTriangles = queue->len;
		}
		while (!isEmpty(queue))
		{
			enqueue(clippedTrianglesToDraw, dequeue(queue));
		}

		free(queue);
	}

	while (!isEmpty(clippedTrianglesToDraw))
	{
		triangle_t t = dequeue(clippedTrianglesToDraw);
		DrawTriangle(renderer, &t, texture);
	}
	free(sortedTriangles);
	free(clippedTrianglesToDraw);
}

void DrawTriangle(SDL_Renderer *renderer, triangle_t *triangle,
				  SDL_Surface *texture)
{
	vec3d_t normal, l1, l2;
	l1 = vec3_sub(&triangle->verts[1], &triangle->verts[0]);

	l2 = vec3_sub(&triangle->verts[2], &triangle->verts[0]);

	normal = vec3_cross(&l1, &l2);

	float length = sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));
	normal = vec3_div(&normal, length);

	vec3d_t light_direction = {0, 0, -1};
	float l = sqrt(vec3_dot(&light_direction, &light_direction));
	light_direction = vec3_div(&light_direction, l);

	float dp = vec3_dot(&normal, &light_direction);

	SDL_SetRenderDrawColor(renderer, triangle->color.r, triangle->color.g,
						   triangle->color.b, SDL_ALPHA_OPAQUE);
	FillTriangle(renderer, triangle);
	FillTriangleWithTexture(renderer, triangle, texture);
	SDL_SetRenderDrawColor(renderer, 250, 250, 250, SDL_ALPHA_OPAQUE);
	DrawWireframeTriangle(renderer, triangle);
}

void DrawWireframeTriangle(SDL_Renderer *renderer, triangle_t *triangle)
{
	SDL_RenderDrawLine(renderer, triangle->verts[0].x, triangle->verts[0].y,
					   triangle->verts[1].x, triangle->verts[1].y);
	SDL_RenderDrawLine(renderer, triangle->verts[1].x, triangle->verts[1].y,
					   triangle->verts[2].x, triangle->verts[2].y);
	SDL_RenderDrawLine(renderer, triangle->verts[2].x, triangle->verts[2].y,
					   triangle->verts[0].x, triangle->verts[0].y);
}

void FillTriangle(SDL_Renderer *renderer, triangle_t *triangle)
{
	int maxY = 0;
	int minY = 0;
	for (int i = 1; i < 3; i++)
	{
		if (triangle->verts[i].y >= triangle->verts[maxY].y)
			maxY = i;
		else if (triangle->verts[i].y <= triangle->verts[minY].y)
			minY = i;
	}
	int midY = 3 - minY - maxY; // it works!

	vec3d_t vMax = {triangle->verts[maxY].x, triangle->verts[maxY].y, 0};

	vec3d_t vMid = {triangle->verts[midY].x, triangle->verts[midY].y, 0};

	vec3d_t vMin = {triangle->verts[minY].x, triangle->verts[minY].y, 0};

	float slopeHypot = (vMax.x - vMin.x) / (vMax.y - vMin.y);
	float xHyp = vMin.x;

	FillTriangleTop(renderer, &vMin, &vMid, slopeHypot, &xHyp);

	FillTriangleBottom(renderer, &vMid, &vMax, slopeHypot, &xHyp);
}

void FillTriangleTop(SDL_Renderer *renderer, vec3d_t *vTop, vec3d_t *vMid,
					 float slopeHyp, float *xHyp)
{
	if (vMid->y - vTop->y < 1)
		return;

	float slopeA = (vTop->x - vMid->x) / (vTop->y - vMid->y);
	float x1 = vTop->x;
	float *a, *b;

	if (slopeA < slopeHyp)
	{
		a = &x1;
		b = xHyp;
	}
	else
	{
		a = xHyp;
		b = &x1;
	}

	for (int y = vTop->y; y < vMid->y; y++)
	{
		for (int x = *a; x < *b; x++)
		{
			SDL_RenderDrawPoint(renderer, x, y);
		}
		x1 += slopeA;
		*xHyp += slopeHyp;
	}
}

void FillTriangleBottom(SDL_Renderer *renderer, vec3d_t *vMid, vec3d_t *vBot,
						float slopeHyp, float *xHyp)
{
	if (vBot->y - vMid->y < 1)
		return;

	float slopeB = (vMid->x - vBot->x) / (vMid->y - vBot->y);
	float x1 = vMid->x;
	float *a, *b;

	if (slopeB > slopeHyp)
	{
		a = &x1;
		b = xHyp;
	}
	else
	{
		a = xHyp;
		b = &x1;
	}

	for (int y = vMid->y; y < vBot->y; y++)
	{
		for (int x = *a; x < *b; x++)
		{
			SDL_RenderDrawPoint(renderer, x, y);
		}
		x1 += slopeB;
		*xHyp += slopeHyp;
	}
}

void FillTriangleWithTexture(SDL_Renderer *renderer, triangle_t *triangle,
							 SDL_Surface *texture)
{
	unsigned char *pixels = (unsigned char *)texture->pixels;

	int maxY = 0;
	int minY = 0;
	for (int i = 1; i < 3; i++)
	{
		if (triangle->verts[i].y >= triangle->verts[maxY].y)
			maxY = i;
		else if (triangle->verts[i].y <= triangle->verts[minY].y)
			minY = i;
	}
	int midY = 3 - minY - maxY; // it works!

	vec3d_t vMax = {triangle->verts[maxY].x, triangle->verts[maxY].y, 0};
	vec2d_t tMax = {triangle->texture[maxY].u, triangle->texture[maxY].v,
					triangle->texture[maxY].w};

	vec3d_t vMid = {triangle->verts[midY].x, triangle->verts[midY].y, 0};
	vec2d_t tMid = {triangle->texture[midY].u, triangle->texture[midY].v,
					triangle->texture[midY].w};

	vec3d_t vMin = {triangle->verts[minY].x, triangle->verts[minY].y, 0};
	vec2d_t tMin = {triangle->texture[minY].u, triangle->texture[minY].v,
					triangle->texture[minY].w};

	int dx1 = vMid.x - vMin.x;
	int dy1 = vMid.y - vMin.y;
	float du1 = tMid.u - tMin.u;
	float dv1 = tMid.v - tMin.v;
	float dw1 = tMid.w - tMin.w;

	int dx2 = vMax.x - vMin.x;
	int dy2 = vMax.y - vMin.y;
	float du2 = tMax.u - tMin.u;
	float dv2 = tMax.v - tMin.v;
	float dw2 = tMax.w - tMin.w;
	float texu, texv, texw;

	float dx1_step = 0, dx2_step = 0, du1_step = 0, du2_step = 0, dv1_step = 0,
		  dv2_step = 0, dw1_step = 0, dw2_step = 0;

	if (dy1)
		dx1_step = dx1 / (float)abs(dy1);
	if (dy2)
		dx2_step = dx2 / (float)abs(dy2);

	if (dy1)
		du1_step = du1 / (float)abs(dy1);
	if (dy2)
		du2_step = du2 / (float)abs(dy2);

	if (dy1)
		dv1_step = dv1 / (float)abs(dy1);
	if (dy2)
		dv2_step = dv2 / (float)abs(dy2);

	if (dy1)
		dw1_step = dw1 / (float)abs(dy1);
	if (dy2)
		dw2_step = dw2 / (float)abs(dy2);

	if (dy1)
	{
		for (int i = vMin.y; i <= vMid.y; i++)
		{
			int x1 = vMin.x + (float)(i - (int)vMin.y) * dx1_step;
			int x2 = vMin.x + (float)(i - (int)vMin.y) * dx2_step;

			float su = tMin.u + (float)(i - (int)vMin.y) * du1_step;
			float eu = tMin.u + (float)(i - (int)vMin.y) * du2_step;

			float sv = tMin.v + (float)(i - (int)vMin.y) * dv1_step;
			float ev = tMin.v + (float)(i - (int)vMin.y) * dv2_step;

			float sw = tMin.w + (float)(i - (int)vMin.y) * dw1_step;
			float ew = tMin.w + (float)(i - (int)vMin.y) * dw2_step;

			if (x1 > x2)
			{
				int temp = x1;
				x1 = x2;
				x2 = temp;

				float tempu = su;
				su = eu;
				eu = tempu;

				float tempv = sv;
				sv = ev;
				ev = tempv;

				float tempw = sw;
				sw = ew;
				ew = tempw;
			}
			texu = su;
			texv = sv;
			texw = sw;

			float tStep = 1 / ((float)(x2 - x1));
			float t = 0;
			for (int j = x1; j < x2; j++)
			{
				texu = (1 - t) * su + t * eu;
				texv = (1 - t) * sv + t * ev;
				texw = (1 - t) * sw + t * ew;

				int u = (texture->w * (texu / texw));
				int v = (texture->h * (texv / texw));

				SDL_Color rgb;
				Uint32 pixel = GetPixel(texture, u, v);
				SDL_GetRGB(pixel, texture->format, &rgb.r, &rgb.g, &rgb.b);
				Uint8 alpha = SDL_ALPHA_OPAQUE; // or use SDL_GetRGBA() if you need the alpha value

				if (texw <= depthBuffer[i * width + j])
				{
					SDL_SetRenderDrawColor(renderer, rgb.r, rgb.g, rgb.b, rgb.a);
					SDL_RenderDrawPoint(renderer, j, i);
					depthBuffer[i * width + j] = texw;
				}
				t += tStep;
			}
		}
	}

	dy1 = vMax.y - vMid.y;
	dx1 = vMax.x - vMid.x;
	du1 = tMax.u - tMid.u;
	dv1 = tMax.v - tMid.v;
	dw1 = tMax.w - tMid.w;

	if (dy1)
		dx1_step = dx1 / (float)abs(dy1);

	if (dy1)
		du1_step = du1 / (float)abs(dy1);

	if (dy1)
		dv1_step = dv1 / (float)abs(dy1);

	if (dy1)
		dw1_step = dw1 / (float)abs(dy1);

	if (dy1)
	{
		for (int i = vMid.y; i <= vMax.y; i++)
		{
			int x1 = vMid.x + (float)(i - vMid.y) * dx1_step;
			int x2 = vMin.x + (float)(i - vMin.y) * dx2_step;

			float su = tMid.u + (float)(i - vMid.y) * du1_step;
			float eu = tMin.u + (float)(i - vMin.y) * du2_step;

			float sv = tMid.v + (float)(i - vMid.y) * dv1_step;
			float ev = tMin.v + (float)(i - vMin.y) * dv2_step;

			float sw = tMid.w + (float)(i - (int)vMid.y) * dw1_step;
			float ew = tMin.w + (float)(i - (int)vMin.y) * dw2_step;

			if (x1 > x2)
			{
				int temp = x1;
				x1 = x2;
				x2 = temp;

				float tempu = su;
				su = eu;
				eu = tempu;

				float tempv = sv;
				sv = ev;
				ev = tempv;

				float tempw = sw;
				sw = ew;
				ew = tempw;
			}
			texu = su;
			texv = sv;
			texw = sw;

			float tStep = 1 / ((float)(x2 - x1));
			float t = 0;
			for (int j = x1; j < x2; j++)
			{
				texu = (1 - t) * su + t * eu;
				texv = (1 - t) * sv + t * ev;
				texw = (1 - t) * sw + t * ew;

				int u = (texture->w * (texu / texw));
				int v = (texture->h * (texv / texw));

				SDL_Color rgb;
				Uint32 pixel = GetPixel(texture, u, v);
				SDL_GetRGB(pixel, texture->format, &rgb.r, &rgb.g, &rgb.b);
				Uint8 alpha = SDL_ALPHA_OPAQUE; // or use SDL_GetRGBA() if you need the alpha value

				if (texw <= depthBuffer[i * width + j])
				{
					SDL_SetRenderDrawColor(renderer, rgb.r, rgb.g, rgb.b, rgb.a);
					SDL_RenderDrawPoint(renderer, j, i);
					depthBuffer[i * width + j] = texw;
				}
				t += tStep;
			}
		}
	}
}

Uint32 GetPixel(SDL_Surface *surface, int x, int y)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	Uint32 pixel = pixels[y * surface->w + x];
	return pixel;
}
// }