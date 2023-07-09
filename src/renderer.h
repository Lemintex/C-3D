#ifndef _RENDERER_H
#define _RENDERER_H

#include <SDL2/SDL_render.h>

#include "mesh.h"
#include "camera.h"
#include "queue.h"

void Render();

void DrawMesh(SDL_Renderer* renderer, mesh_t* mesh);

void DrawTriangle(SDL_Renderer* renderer, triangle_t* triangle);

void DrawWireframeTriangle(SDL_Renderer* renderer, triangle_t* triangle);

void FillTriangle(SDL_Renderer* renderer, triangle_t* triangle);

void FillTriangleTop(SDL_Renderer* renderer, vec3d_t* vTop, vec3d_t* vMid, float slopeHyp, float* xHyp);

void FillTriangleBottom(SDL_Renderer* renderer, vec3d_t* vTop, vec3d_t* vMid, float slopeHyp, float* xHyp);

void FillTriangleWithTexture(triangle_t* triangle, SDL_Texture* texture);

#endif
