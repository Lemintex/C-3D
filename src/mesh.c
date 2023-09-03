#include "mesh.h"

mesh_t *ReadMeshFromFile(char *filename, int hasTexture)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Error: Unable to open file %s\n", filename);
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
		if (!hasTexture)
		{
			if (line[0] == 'f' && line[1] == ' ')
			{
				int verts[3];
				sscanf(line, "f %d %d %d", &verts[0], &verts[1], &verts[2]);
				triangle_t face;
				face = (triangle_t){vertices[verts[0] - 1], vertices[verts[1] - 1], vertices[verts[2] - 1]};
				faces = (triangle_t *)realloc(faces, (face_count + 1) * sizeof(triangle_t));
				faces[face_count] = face;
				face_count++;
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
				faces[face_count] = face;
				face_count++;
				int nTokens = 0;
				char *token = strtok(line + 2, " \t\n");
				while (token != NULL)
				{
					// Check if the token contains a '/'
					char *slash = strchr(token, '/');
					if (slash != NULL)
					{
						printf("Found 'f/t' pair: %s\n", token);
					}
					token = strtok(NULL, " \t\n");
				}
			}
		}
	}

	fclose(file);

	// Process the vertices and faces as needed
	// ...
	mesh_t *mesh = (mesh_t *)malloc(sizeof(mesh_t));
	mesh->triangleCount = face_count;
	mesh->triangles = faces; //(triangle_t)malloc(sizeof(triangle_t) * mesh->triangleCount;

	// Free the allocated memory
	free(vertices);
	free(textures);
	return mesh;
}

color_t createColor(unsigned char r, unsigned char g, unsigned char b)
{
	color_t color;
	color.r = r;
	color.g = g;
	color.b = b;
	return color;
}

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

int triangle_clipAgainstPlane(vec3d_t *planePoint, vec3d_t *planeNormal, triangle_t *triangleIn, triangle_t *triangleOut1, triangle_t *triangleOut2)
{
	*planeNormal = vec3_normal(planeNormal);

	float distance[3];
	// gets signed distance from planePoint to triangle point
	for (int i = 0; i < 3; i++)
	{
		vec3d_t v = triangleIn->verts[i];
		v = vec3_add(&v, planePoint);
		distance[i] = vec3_dot(planeNormal, &v); // planePoint) - (planeNormal->x * v.x +planeNormal->y * v.y + planeNormal->z * v.z);
	}

	// create temporary inside and outside arrays used to classify points
	vec3d_t *inside_points[3];
	int nInsidePointCount = 0;
	vec3d_t *outside_points[3];
	int nOutsidePointCount = 0;

	vec2d_t *inside_texture[3];
	int nInsideTextureCount = 0;
	vec2d_t *outside_texture[3];
	int nOutsideTextureCount = 0;

	for (int i = 0; i < 3; i++)
	{
		// if distance is more than 0, point is 'inside' plane
		if (distance[i] < 0)
		{
			inside_points[nInsidePointCount++] = &triangleIn->verts[i];
			inside_texture[nInsideTextureCount++] = &triangleIn->texture[i];
		}
		// oherwise, it is 'outside' plane
		else
		{
			outside_points[nOutsidePointCount++] = &triangleIn->verts[i];
			outside_texture[nOutsideTextureCount++] = &triangleIn->texture[i];
		}
	}

	if (nInsidePointCount == 0)
		return 0;

	if (nInsidePointCount == 3)
	{
		*triangleOut1 = *triangleIn;

		//		triangleOut1->color = createColor(0, 0, 255);
		return 1;
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		triangleOut1->verts[0] = *inside_points[0];
		triangleOut1->texture[0] = *inside_texture[0];
		float t;
		triangleOut1->verts[1] = vec3_intersectPlane(planePoint, planeNormal, inside_points[0], outside_points[0], &t);
		triangleOut1->texture[1].u = t * (outside_texture[0]->u - inside_texture[0]->u) + inside_texture[0]->u;
		triangleOut1->texture[1].v = t * (outside_texture[0]->v - inside_texture[0]->v) + inside_texture[0]->v;
		triangleOut1->texture[1].w = t * (outside_texture[0]->w - inside_texture[0]->w) + inside_texture[0]->w;

		triangleOut1->verts[2] = vec3_intersectPlane(planePoint, planeNormal, inside_points[0], outside_points[1], &t);
		triangleOut1->texture[2].u = t * (outside_texture[1]->u - inside_texture[0]->u) + inside_texture[0]->u;
		triangleOut1->texture[2].v = t * (outside_texture[1]->v - inside_texture[0]->v) + inside_texture[0]->v;
		triangleOut1->texture[2].w = t * (outside_texture[1]->w - inside_texture[0]->w) + inside_texture[0]->w;

		triangleOut1->color = createColor(0, 255, 0);
		return 1;
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		triangleOut1->verts[0] = *inside_points[0];
		triangleOut1->verts[1] = *inside_points[1];

		triangleOut1->texture[0] = *inside_texture[0];
		triangleOut1->texture[1] = *inside_texture[1];

		float t;
		triangleOut1->verts[2] = vec3_intersectPlane(planePoint, planeNormal, inside_points[0], outside_points[0], &t);

		triangleOut1->texture[2].u = t * (outside_texture[0]->u - inside_texture[0]->u) + inside_texture[0]->u;
		triangleOut1->texture[2].v = t * (outside_texture[0]->v - inside_texture[0]->v) + inside_texture[0]->v;
		triangleOut1->texture[2].w = t * (outside_texture[0]->w - inside_texture[0]->w) + inside_texture[0]->w;

		triangleOut1->color = createColor(255, 0, 0);

		triangleOut2->verts[0] = *inside_points[1];
		triangleOut2->verts[1] = triangleOut1->verts[2];

		triangleOut2->texture[0] = *inside_texture[1];
		triangleOut2->texture[1] = triangleOut1->texture[2];

		triangleOut2->verts[2] = vec3_intersectPlane(planePoint, planeNormal, inside_points[1], outside_points[0], &t);

		triangleOut2->texture[2].u = t * (outside_texture[0]->u - inside_texture[1]->u) + inside_texture[1]->u;
		triangleOut2->texture[2].v = t * (outside_texture[0]->v - inside_texture[1]->v) + inside_texture[1]->v;
		triangleOut2->texture[2].w = t * (outside_texture[0]->w - inside_texture[1]->w) + inside_texture[1]->w;
		triangleOut2->color = createColor(255, 0, 0);

		return 2;
	}
}
