#ifndef _RENDERER_H
#define _RENDERER_H

#include <SDL2/SDL_render.h>

#include "3dmath.h"
#include "camera.h"
#include "mesh.h"
#include "options.h"
#include "queue.h"

void render();

void draw_mesh(SDL_Renderer *renderer, mesh_t *mesh, SDL_Surface *texture);

void draw_triangle(SDL_Renderer *renderer, triangle_t *triangle,
                   SDL_Surface *texture);

void draw_wireframe_triangle(SDL_Renderer *renderer, triangle_t *triangle);

void fill_triangle(SDL_Renderer *renderer, triangle_t *triangle);

void fill_triangle_half(SDL_Renderer *renderer, vec3d_t *v_top, vec3d_t *v_mid,
                        float slope_hyp, float *x_hyp, int is_top);

void fill_triangle_with_texture(SDL_Renderer *renderer, triangle_t *triangle,
                                SDL_Surface *texture);

Uint32 get_pixel(SDL_Surface *surface, int x, int y);

void render_mode();
#endif
