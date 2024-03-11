#include "renderer.h"
#include "3dmath.h"

#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>

extern options_t options;
extern camera_t camera;
extern int width, height;
extern float *depth_buffer;

void draw_mesh(SDL_Renderer *renderer, mesh_t *mesh, SDL_Surface *texture) {
  int triangles_to_draw = 0;
  triangle_t *sorted_triangles = (triangle_t *)malloc(0);

  matrix_4x4_t mat_proj = matrix_projection(90, width / height, 0.1, 1000);

  static float delta = 0;
  matrix_4x4_t mat_rot_z = matrix_rotation_z(delta);
  matrix_4x4_t mat_rot_x = matrix_rotation_x(delta);
  matrix_4x4_t mat_rot_a;
  matrix_4x4_t mat_trans = matrix_translation(5, 0, 10);

  matrix_4x4_t mat_world = matrix_identity();
  mat_rot_a = matrix_multiply_matrix(&mat_rot_z, &mat_rot_x);

  mat_world = matrix_multiply_matrix(&mat_world, &mat_trans);

  vec3d_t vRightTemp = (vec3d_t){1, 0, 0, 1};
  vec3d_t vUpTemp = (vec3d_t){0, 1, 0, 1};
  vec3d_t vLookTemp = (vec3d_t){0, 0, 1, 1};

  vec3d_t target = (vec3d_t){0, 0, 1, 1};
  matrix_4x4_t yaw = matrix_rotation_y(camera.yaw);
  matrix_4x4_t pitch = matrix_rotation_x(camera.pitch);

  matrix_4x4_t look = matrix_multiply_matrix(&pitch, &yaw);
  camera.look_dir.forward = vec3_mul_mat4(&vLookTemp, &look);
  camera.look_dir.up = vec3_mul_mat4(&vUpTemp, &look);
  camera.look_dir.right =
      vec3_cross(&camera.look_dir.forward, &camera.look_dir.up);

  target = vec3_add(&camera.pos, &camera.look_dir.forward);

  matrix_4x4_t camera_matrix =
      matrix_point_at(&camera.pos, &target, &camera.look_dir.up);

  matrix_4x4_t camera_view = matrix_quick_inverse(&camera_matrix);

  for (int i = 0; i < mesh->triangle_count; i++) {
    triangle_t triangle = mesh->triangles[i];

    triangle_t triangle_projected;
    triangle_t triangle_transformed;
    triangle_t triangle_viewed;

    triangle_transformed = triangle_mul_mat4(&triangle, &mat_world);

    *triangle_transformed.texture = *triangle.texture;

    // CALCULATE COLOUR OF TRIANGLE
    vec3d_t normal, l1, l2;

    l1 = vec3_sub(&triangle_transformed.verts[1],
                  &triangle_transformed.verts[0]);
    l2 = vec3_sub(&triangle_transformed.verts[2],
                  &triangle_transformed.verts[0]);

    normal = vec3_cross(&l1, &l2);

    normal = vec3_normal(&normal);

    vec3d_t ray_from_camera =
        vec3_sub(&triangle_transformed.verts[1], &camera.pos);

    if (vec3_dot(&normal, &ray_from_camera) >= 0) {
      continue;
    }
    vec3d_t light_direction = {0, 0, -1};
    light_direction = vec3_normal(&light_direction);

    float dp = vec3_dot(&light_direction, &normal);

    triangle_viewed = triangle_mul_mat4(&triangle_transformed, &camera_view);

    *triangle_viewed.texture = *triangle_transformed.texture;

    // clip against near plane of camera
    int clipped_triangles = 0;
    triangle_t clipped[2];

    vec3d_t near_plane = (vec3d_t){0, 0, 0.1, 1};
    vec3d_t near_plane_normal = (vec3d_t){0, 0, 1, 1}; // FRONT PLANE

    clipped_triangles =
        triangle_clip_against_plane(&near_plane, &near_plane_normal,
                                    &triangle_viewed, &clipped[0], &clipped[1]);

    for (int j = 0; j < clipped_triangles; j++) {
      triangle_projected = triangle_mul_mat4(&clipped[j], &mat_proj);

      *triangle_projected.texture = *clipped[j].texture;

      triangle_projected.texture[0].u /= triangle_projected.verts[0].w;
      triangle_projected.texture[1].u /= triangle_projected.verts[1].w;
      triangle_projected.texture[2].u /= triangle_projected.verts[2].w;

      triangle_projected.texture[0].v /= triangle_projected.verts[0].w;
      triangle_projected.texture[1].v /= triangle_projected.verts[1].w;
      triangle_projected.texture[2].v /= triangle_projected.verts[2].w;

      triangle_projected.texture[0].w = 1.0 / triangle_projected.verts[0].w;
      triangle_projected.texture[1].w = 1.0 / triangle_projected.verts[1].w;
      triangle_projected.texture[2].w = 1.0 / triangle_projected.verts[2].w;

      // normalise co-ordinates
      triangle_projected.verts[0] =
          vec3_div(&triangle_projected.verts[0], triangle_projected.verts[0].w);
      triangle_projected.verts[1] =
          vec3_div(&triangle_projected.verts[1], triangle_projected.verts[1].w);
      triangle_projected.verts[2] =
          vec3_div(&triangle_projected.verts[2], triangle_projected.verts[2].w);

      uint8_t shade = dp * 200 + 55;
      triangle_projected.color = create_color(shade, shade, shade);

      // offset into view
      vec3d_t vOffsetView = (vec3d_t){1, 1, 0};

      triangle_projected.verts[0] =
          vec3_add(&triangle_projected.verts[0], &vOffsetView);
      triangle_projected.verts[1] =
          vec3_add(&triangle_projected.verts[1], &vOffsetView);
      triangle_projected.verts[2] =
          vec3_add(&triangle_projected.verts[2], &vOffsetView);

      triangle_projected.verts[0].x *= 0.5 * width;
      triangle_projected.verts[0].y *= 0.5 * height;

      triangle_projected.verts[1].x *= 0.5 * width;
      triangle_projected.verts[1].y *= 0.5 * height;

      triangle_projected.verts[2].x *= 0.5 * width;
      triangle_projected.verts[2].y *= 0.5 * height;

      triangles_to_draw++;
      sorted_triangles = (triangle_t *)realloc(
          sorted_triangles, sizeof(triangle_t) * triangles_to_draw);
      sorted_triangles[triangles_to_draw - 1] = triangle_projected;
    }
  }

  qsort(sorted_triangles, triangles_to_draw, sizeof(triangle_t), compareZ);

  queue_t *clipped_triangles_to_draw = create_queue();

  for (int i = 0; i < triangles_to_draw; i++) {
    triangle_t clipped[2];

    queue_t *queue = create_queue();
    enqueue(queue, sorted_triangles[i]);

    int n_new_triangles = 1;

    for (int p = 0; p < 4; p++) {
      int triangles_to_add = 1;
      while (n_new_triangles > 0) {
        n_new_triangles--;
        if (is_empty(queue)) {
          continue;
        }
        triangle_t test = dequeue(queue);

        switch (p) {
        case 0:
          triangles_to_add = triangle_clip_against_plane(
              &((vec3d_t){0, 0, 0, 1}), &((vec3d_t){0, -1, 0, 1}), &test,
              &clipped[0], &clipped[1]); // TOP
          break;

        case 1:
          triangles_to_add = triangle_clip_against_plane(
              &((vec3d_t){0, -width, 0, 1}), &((vec3d_t){0, 1, 0, 1}), &test,
              &clipped[0], &clipped[1]); // BOTTOM
          break;

        case 2:
          triangles_to_add = triangle_clip_against_plane(
              &((vec3d_t){0, 0, 0, 1}), &((vec3d_t){-1, 0, 0, 1}), &test,
              &clipped[0], &clipped[1]); // LEFT
          break;

        case 3:
          triangles_to_add = triangle_clip_against_plane(
              &((vec3d_t){-height, 0, 0, 1}), &((vec3d_t){1, 0, 0, 1}), &test,
              &clipped[0], &clipped[1]); // RIGHT
          break;

        default:
          break;
        }

        for (int t = 0; t < triangles_to_add; t++) {
          enqueue(queue, clipped[t]);
        }
      }
      n_new_triangles = queue->len;
    }
    while (!is_empty(queue)) {
      enqueue(clipped_triangles_to_draw, dequeue(queue));
    }

    free(queue);
  }

  while (!is_empty(clipped_triangles_to_draw)) {
    triangle_t t = dequeue(clipped_triangles_to_draw);
    draw_triangle(renderer, &t, texture);
  }
  free(sorted_triangles);
  free(clipped_triangles_to_draw);
}

void draw_triangle(SDL_Renderer *renderer, triangle_t *triangle,
                   SDL_Surface *texture) {
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

  switch (options.display_type) {
  case WIREFRAME:
    SDL_SetRenderDrawColor(renderer, 250, 250, 250, SDL_ALPHA_OPAQUE);
    draw_wireframe_triangle(renderer, triangle);
    break;

  case MONOCHROME:
    fill_triangle(renderer, triangle);
    break;

  case TEXTURE:
    fill_triangle_with_texture(renderer, triangle, texture);
    break;
  }
}

void draw_wireframe_triangle(SDL_Renderer *renderer, triangle_t *triangle) {
  SDL_RenderDrawLine(renderer, triangle->verts[0].x, triangle->verts[0].y,
                     triangle->verts[1].x, triangle->verts[1].y);
  SDL_RenderDrawLine(renderer, triangle->verts[1].x, triangle->verts[1].y,
                     triangle->verts[2].x, triangle->verts[2].y);
  SDL_RenderDrawLine(renderer, triangle->verts[2].x, triangle->verts[2].y,
                     triangle->verts[0].x, triangle->verts[0].y);
}

void fill_triangle(SDL_Renderer *renderer, triangle_t *triangle) {
  int max_y = 0;
  int min_y = 0;
  for (int i = 1; i < 3; i++) {
    if (triangle->verts[i].y >= triangle->verts[max_y].y) {
      max_y = i;
    } else if (triangle->verts[i].y <= triangle->verts[min_y].y) {
      min_y = i;
    }
  }
  int mid_y = 3 - min_y - max_y; // it works!

  vec3d_t v_max = {triangle->verts[max_y].x, triangle->verts[max_y].y, 0};

  vec3d_t v_mid = {triangle->verts[mid_y].x, triangle->verts[mid_y].y, 0};

  vec3d_t v_min = {triangle->verts[min_y].x, triangle->verts[min_y].y, 0};

  float slope_hypot = (v_max.x - v_min.x) / (v_max.y - v_min.y);
  float x_hyp = v_min.x;

  fill_triangle_half(renderer, &v_min, &v_mid, slope_hypot, &x_hyp, 1);

  fill_triangle_half(renderer, &v_mid, &v_max, slope_hypot, &x_hyp, 0);
}

void fill_triangle_half(SDL_Renderer *renderer, vec3d_t *v1, vec3d_t *v2,
                        float slope_hyp, float *x_hyp, int isTop) {
  if (v2->y - v1->y < 1) {
    return;
  }

  float slope_a = (v1->x - v2->x) / (v1->y - v2->y);
  float x1 = v1->x;
  float *a, *b;
  int result = 0;
  if (isTop) {
    result = slope_a < slope_hyp;
  } else {
    result = slope_a > slope_hyp;
  }
  if (result) {
    a = &x1;
    b = x_hyp;
  } else {
    a = x_hyp;
    b = &x1;
  }

  for (int y = v1->y; y < v2->y; y++) {
    for (int x = *a; x < *b; x++) {
      SDL_RenderDrawPoint(renderer, x, y);
    }
    x1 += slope_a;
    *x_hyp += slope_hyp;
  }
}

void fill_triangle_with_texture(SDL_Renderer *renderer, triangle_t *triangle,
                                SDL_Surface *texture) {
  unsigned char *pixels = (unsigned char *)texture->pixels;

  int max_y = 0;
  int min_y = 0;
  for (int i = 1; i < 3; i++) {
    if (triangle->verts[i].y >= triangle->verts[max_y].y) {
      max_y = i;
    } else if (triangle->verts[i].y <= triangle->verts[min_y].y) {
      min_y = i;
    }
  }
  int mid_y = 3 - min_y - max_y; // it works!

  vec3d_t v_max = {triangle->verts[max_y].x, triangle->verts[max_y].y, 0};
  vec2d_t t_max = {triangle->texture[max_y].u, triangle->texture[max_y].v,
                   triangle->texture[max_y].w};

  vec3d_t v_mid = {triangle->verts[mid_y].x, triangle->verts[mid_y].y, 0};
  vec2d_t t_mid = {triangle->texture[mid_y].u, triangle->texture[mid_y].v,
                   triangle->texture[mid_y].w};

  vec3d_t v_min = {triangle->verts[min_y].x, triangle->verts[min_y].y, 0};
  vec2d_t t_min = {triangle->texture[min_y].u, triangle->texture[min_y].v,
                   triangle->texture[min_y].w};

  int dx1 = v_mid.x - v_min.x;
  int dy1 = v_mid.y - v_min.y;
  float du1 = t_mid.u - t_min.u;
  float dv1 = t_mid.v - t_min.v;
  float dw1 = t_mid.w - t_min.w;

  int dx2 = v_max.x - v_min.x;
  int dy2 = v_max.y - v_min.y;
  float du2 = t_max.u - t_min.u;
  float dv2 = t_max.v - t_min.v;
  float dw2 = t_max.w - t_min.w;
  float texu, texv, texw;

  float dx1_step = 0, dx2_step = 0, du1_step = 0, du2_step = 0, dv1_step = 0,
        dv2_step = 0, dw1_step = 0, dw2_step = 0;

  if (dy1) {
    dx1_step = dx1 / (float)abs(dy1);
  }
  if (dy2) {
    dx2_step = dx2 / (float)abs(dy2);
  }

  if (dy1) {
    du1_step = du1 / (float)abs(dy1);
  }
  if (dy2) {
    du2_step = du2 / (float)abs(dy2);
  }

  if (dy1) {
    dv1_step = dv1 / (float)abs(dy1);
  }
  if (dy2) {
    dv2_step = dv2 / (float)abs(dy2);
  }

  if (dy1) {
    dw1_step = dw1 / (float)abs(dy1);
  }
  if (dy2) {
    dw2_step = dw2 / (float)abs(dy2);
  }

  if (dy1) {
    for (int i = v_min.y; i <= v_mid.y; i++) {
      int x1 = v_min.x + (float)(i - (int)v_min.y) * dx1_step;
      int x2 = v_min.x + (float)(i - (int)v_min.y) * dx2_step;

      float su = t_min.u + (float)(i - (int)v_min.y) * du1_step;
      float eu = t_min.u + (float)(i - (int)v_min.y) * du2_step;

      float sv = t_min.v + (float)(i - (int)v_min.y) * dv1_step;
      float ev = t_min.v + (float)(i - (int)v_min.y) * dv2_step;

      float sw = t_min.w + (float)(i - (int)v_min.y) * dw1_step;
      float ew = t_min.w + (float)(i - (int)v_min.y) * dw2_step;

      if (x1 > x2) {
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

      float t_step = 1 / ((float)(x2 - x1));
      float t = 0;
      for (int j = x1; j < x2; j++) {
        texu = (1 - t) * su + t * eu;
        texv = (1 - t) * sv + t * ev;
        texw = (1 - t) * sw + t * ew;

        int u = (texture->w * (texu / texw));
        int v = (texture->h * (texv / texw));

        SDL_Color rgb;
        Uint32 pixel = get_pixel(texture, u, v);
        SDL_GetRGB(pixel, texture->format, &rgb.r, &rgb.g, &rgb.b);
        Uint8 alpha = SDL_ALPHA_OPAQUE;

        if (texw <= depth_buffer[i * width + j]) {
          SDL_SetRenderDrawColor(renderer, rgb.r, rgb.g, rgb.b, rgb.a);
          SDL_RenderDrawPoint(renderer, j, i);
          depth_buffer[i * width + j] = texw;
        }
        t += t_step;
      }
    }
  }

  dy1 = v_max.y - v_mid.y;
  dx1 = v_max.x - v_mid.x;
  du1 = t_max.u - t_mid.u;
  dv1 = t_max.v - t_mid.v;
  dw1 = t_max.w - t_mid.w;

  if (dy1) {
    dx1_step = dx1 / (float)abs(dy1);
  }

  if (dy1) {
    du1_step = du1 / (float)abs(dy1);
  }

  if (dy1) {
    dv1_step = dv1 / (float)abs(dy1);
  }

  if (dy1) {
    dw1_step = dw1 / (float)abs(dy1);
  }

  if (dy1) {
    for (int i = v_mid.y; i <= v_max.y; i++) {
      int x1 = v_mid.x + (float)(i - v_mid.y) * dx1_step;
      int x2 = v_min.x + (float)(i - v_min.y) * dx2_step;

      float su = t_mid.u + (float)(i - v_mid.y) * du1_step;
      float eu = t_min.u + (float)(i - v_min.y) * du2_step;

      float sv = t_mid.v + (float)(i - v_mid.y) * dv1_step;
      float ev = t_min.v + (float)(i - v_min.y) * dv2_step;

      float sw = t_mid.w + (float)(i - (int)v_mid.y) * dw1_step;
      float ew = t_min.w + (float)(i - (int)v_min.y) * dw2_step;

      if (x1 > x2) {
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

      float t_step = 1 / ((float)(x2 - x1));
      float t = 0;
      for (int j = x1; j < x2; j++) {
        texu = (1 - t) * su + t * eu;
        texv = (1 - t) * sv + t * ev;
        texw = (1 - t) * sw + t * ew;

        int u = (texture->w * (texu / texw));
        int v = (texture->h * (texv / texw));

        SDL_Color rgb;
        Uint32 pixel = get_pixel(texture, u, v);
        SDL_GetRGB(pixel, texture->format, &rgb.r, &rgb.g, &rgb.b);
        Uint8 alpha = SDL_ALPHA_OPAQUE; // or use SDL_GetRGBA() if you need the
                                        // alpha value

        if (texw <= depth_buffer[i * width + j]) {
          SDL_SetRenderDrawColor(renderer, rgb.r, rgb.g, rgb.b, rgb.a);
          SDL_RenderDrawPoint(renderer, j, i);
          depth_buffer[i * width + j] = texw;
        }
        t += t_step;
      }
    }
  }
}

Uint32 get_pixel(SDL_Surface *surface, int x, int y) {
  Uint32 *pixels = (Uint32 *)surface->pixels;
  Uint32 pixel = pixels[y * surface->w + x];
  return pixel;
}
