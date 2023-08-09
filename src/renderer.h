#ifndef _RENDERER_H
#define _RENDERER_H

#include <SDL2/SDL_render.h>

#include "mesh.h"
#include "camera.h"
#include "queue.h"

void Render();

void DrawMesh(SDL_Renderer* renderer, mesh_t* mesh, SDL_Surface* texture);

void DrawTriangle(SDL_Renderer* renderer, triangle_t* triangle, SDL_Surface* texture);

void DrawWireframeTriangle(SDL_Renderer* renderer, triangle_t* triangle);

void FillTriangle(SDL_Renderer* renderer, triangle_t* triangle);

void FillTriangleTop(SDL_Renderer* renderer, vec3d_t* vTop, vec3d_t* vMid, float slopeHyp, float* xHyp);

void FillTriangleBottom(SDL_Renderer* renderer, vec3d_t* vTop, vec3d_t* vMid, float slopeHyp, float* xHyp);

void FillTriangleWithTexture(SDL_Renderer* renderer, triangle_t* triangle, SDL_Surface* texture);

void TextureTriangleHalf(SDL_Renderer* renderer, SDL_Surface* texture, vec3d_t vec1, vec3d_t vec2, vec2d_t tex1, vec2d_t tex2, int dx1, int dy1, float du1, float dv1, float dw1, float* dx_step, float* du_step, float* dv_step, float* dw_step);

#endif
