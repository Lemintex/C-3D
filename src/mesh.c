#include "mesh.h"

mesh_t *read_mesh_from_file(char *filename, int has_texture)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Error: Unable to open file %s. Is the file in the correct folder?\n", filename);
		return NULL;
	}

	vec3d_t *vertices = NULL;
	vec2d_t *textures = NULL;
	triangle_t *faces = NULL;

	int texture_count = 0;
	int vertex_count = 0;
	int face_count = 0;

	char line[256];
	while (fgets(line, sizeof(line), file))
	{
		if (line[0] == 'v')
		{
			if (line[1] == 't')
			{
				vec2d_t texture;
				sscanf(line, "vt %f %f", &texture.u, &texture.v);

				// Depending on the sprite and the orientation of the map, we
				// may have to invert either the u, v, or both coordinates
				texture.v = 1 - texture.v;

				texture.w = 1;

				textures = (vec2d_t *)realloc(textures, (texture_count + 1) * sizeof(vec2d_t));
				textures[texture_count++] = texture;
			}
			else
			{
				vec3d_t vertex;
				sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);

				vertex.w = 1;

				vertices = (vec3d_t *)realloc(vertices, (vertex_count + 1) * sizeof(vec3d_t));
				vertices[vertex_count++] = vertex;
			}
		}
		if (!has_texture)
		{
			if (line[0] == 'f' && line[1] == ' ')
			{
				int verts[3];
				sscanf(line, "f %d %d %d", &verts[0], &verts[1], &verts[2]);

				triangle_t face;
				face = (triangle_t){vertices[verts[0] - 1], vertices[verts[1] - 1], vertices[verts[2] - 1]};

				faces = (triangle_t *)realloc(faces, (face_count + 1) * sizeof(triangle_t));
				faces[face_count++] = face;
			}
		}
		else
		{
			if (line[0] == 'f')
			{
				int tokens[6];
				sscanf(line, "f %d/%d %d/%d %d/%d", &tokens[0], &tokens[1], &tokens[2], &tokens[3], &tokens[4], &tokens[5]);

				triangle_t face;
				face = (triangle_t){vertices[tokens[0] - 1], vertices[tokens[2] - 1], vertices[tokens[4] - 1], textures[tokens[1] - 1], textures[tokens[3] - 1], textures[tokens[5] - 1]};

				faces = (triangle_t *)realloc(faces, (face_count + 1) * sizeof(triangle_t));
				faces[face_count++] = face;
			}
		}
	}

	fclose(file);

	// Process the vertices and faces as needed
	// ...
	mesh_t *mesh = (mesh_t *)malloc(sizeof(mesh_t));
	mesh->triangle_count = face_count;
	mesh->triangles = faces;

	// Free the allocated memory
	free(vertices);
	free(textures);
	return mesh;
}

color_t create_color(unsigned char r, unsigned char g, unsigned char b)
{
	color_t color;
	color.r = r;
	color.g = g;
	color.b = b;
	return color;
}

// REMOVE THIS? NOT NEEDED NOW DEPTH BUFFER IS FUNCTIONAL?
int compareZ(const void *e1, const void *e2)
{
	triangle_t *a = (triangle_t *)e1;
	float averageA = a->verts[0].z + a->verts[1].z + a->verts[2].z / 3;

	triangle_t *b = (triangle_t *)e2;
	float averageB = b->verts[0].z + b->verts[1].z + b->verts[2].z / 3;

	if (averageA < averageB)
		return -1;
	if (averageA > averageB)
		return 1;
	return 0;
}

int triangle_clip_against_plane(vec3d_t *plane_point, vec3d_t *plane_normal, triangle_t *triangle_in, triangle_t *triangle_out_1, triangle_t *triangle_out_2)
{
	*plane_normal = vec3_normal(plane_normal);

	float distance[3];
	// gets signed distance from plane_point to triangle point
	for (int i = 0; i < 3; i++)
	{
		vec3d_t v = triangle_in->verts[i];
		v = vec3_add(&v, plane_point);
		distance[i] = vec3_dot(plane_normal, &v); // plane_point) - (plane_normal->x * v.x +plane_normal->y * v.y + plane_normal->z * v.z);
	}

	// create temporary inside and outside arrays used to classify points
	vec3d_t *inside_points[3];
	int n_inside_point_count = 0;
	vec3d_t *outside_points[3];
	int n_outside_point_count = 0;

	vec2d_t *inside_texture[3];
	int n_inside_texture_count = 0;
	vec2d_t *outside_texture[3];
	int n_outside_texture_count = 0;

	for (int i = 0; i < 3; i++)
	{
		// if distance is more than 0, point is 'inside' plane
		if (distance[i] < 0)
		{
			inside_points[n_inside_point_count++] = &triangle_in->verts[i];
			inside_texture[n_inside_texture_count++] = &triangle_in->texture[i];
		}
		// otherwise, it is 'outside' plane
		else
		{
			outside_points[n_outside_point_count++] = &triangle_in->verts[i];
			outside_texture[n_outside_texture_count++] = &triangle_in->texture[i];
		}
	}

	if (n_inside_point_count == 0)
		return 0;

	if (n_inside_point_count == 3)
	{
		*triangle_out_1 = *triangle_in;

		return 1;
	}

	if (n_inside_point_count == 1 && n_outside_point_count == 2)
	{
		triangle_out_1->verts[0] = *inside_points[0];
		triangle_out_1->texture[0] = *inside_texture[0];
		float t;

		triangle_out_1->verts[1] = vec3_intersect_plane(plane_point, plane_normal, inside_points[0], outside_points[0], &t);

		triangle_out_1->texture[1].u = t * (outside_texture[0]->u - inside_texture[0]->u) + inside_texture[0]->u;
		triangle_out_1->texture[1].v = t * (outside_texture[0]->v - inside_texture[0]->v) + inside_texture[0]->v;
		triangle_out_1->texture[1].w = t * (outside_texture[0]->w - inside_texture[0]->w) + inside_texture[0]->w;

		triangle_out_1->verts[2] = vec3_intersect_plane(plane_point, plane_normal, inside_points[0], outside_points[1], &t);

		triangle_out_1->texture[2].u = t * (outside_texture[1]->u - inside_texture[0]->u) + inside_texture[0]->u;
		triangle_out_1->texture[2].v = t * (outside_texture[1]->v - inside_texture[0]->v) + inside_texture[0]->v;
		triangle_out_1->texture[2].w = t * (outside_texture[1]->w - inside_texture[0]->w) + inside_texture[0]->w;

		return 1;
	}

	if (n_inside_point_count == 2 && n_outside_point_count == 1)
	{
		triangle_out_1->verts[0] = *inside_points[0];
		triangle_out_1->verts[1] = *inside_points[1];

		triangle_out_1->texture[0] = *inside_texture[0];
		triangle_out_1->texture[1] = *inside_texture[1];

		float t;
		triangle_out_1->verts[2] = vec3_intersect_plane(plane_point, plane_normal, inside_points[0], outside_points[0], &t);

		triangle_out_1->texture[2].u = t * (outside_texture[0]->u - inside_texture[0]->u) + inside_texture[0]->u;
		triangle_out_1->texture[2].v = t * (outside_texture[0]->v - inside_texture[0]->v) + inside_texture[0]->v;
		triangle_out_1->texture[2].w = t * (outside_texture[0]->w - inside_texture[0]->w) + inside_texture[0]->w;

		triangle_out_1->color = create_color(255, 0, 0);

		triangle_out_2->verts[0] = *inside_points[1];
		triangle_out_2->verts[1] = triangle_out_1->verts[2];

		triangle_out_2->texture[0] = *inside_texture[1];
		triangle_out_2->texture[1] = triangle_out_1->texture[2];

		triangle_out_2->verts[2] = vec3_intersect_plane(plane_point, plane_normal, inside_points[1], outside_points[0], &t);

		triangle_out_2->texture[2].u = t * (outside_texture[0]->u - inside_texture[1]->u) + inside_texture[1]->u;
		triangle_out_2->texture[2].v = t * (outside_texture[0]->v - inside_texture[1]->v) + inside_texture[1]->v;
		triangle_out_2->texture[2].w = t * (outside_texture[0]->w - inside_texture[1]->w) + inside_texture[1]->w;

		return 2;
	}
}
