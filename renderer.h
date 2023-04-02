#ifndef _RENDERER_H
#define _RENDERER_H

#include <SDL2/SDL_render.h>

#include "mesh.h"


void Render();

void DrawMesh(SDL_Renderer* renderer, mesh_t* mesh);

void DrawTriangle(SDL_Renderer* renderer, triangle_t* triangle);

#endif
