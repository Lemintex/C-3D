#include "mesh.h"

mesh_t* ReadMeshFromFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return NULL;
    }

    vec3d_t* vertices = NULL;
    triangle_t* faces = NULL;
    int vertex_count = 0;
    int face_count = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            vec3d_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			vertex.w = 1;
            vertices = (vec3d_t*)realloc(vertices, (vertex_count + 1) * sizeof(vec3d_t));
            vertices[vertex_count++] = vertex;
        }
		else if (line[0] == 'f' && line[1] == ' ') {
			int verts[3];
            sscanf(line, "f %d %d %d", &verts[0], &verts[1], &verts[2]);
			triangle_t face;
			face = (triangle_t){vertices[verts[0] - 1], vertices[verts[1] - 1], vertices[verts[2] - 1]};
            faces = (triangle_t*)realloc(faces, (face_count + 1) * sizeof(triangle_t));
            faces[face_count] = face;
			face_count++;
        }
    }

    fclose(file);

    // Process the vertices and faces as needed
    // ...
	mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t));
	mesh->triangleCount = face_count;
	mesh->triangles = faces;//(triangle_t)malloc(sizeof(triangle_t) * mesh->triangleCount;

    // Free the allocated memory
    free(vertices);


	return mesh;
}

color_t createColor(u_int8_t r, u_int8_t g, u_int8_t b) {
	color_t color;
	color.r = r;
	color.g = g;
	color.b = b;
	return color;
}

int compareZ(const void* e1, const void* e2) {
	triangle_t* a = (triangle_t*)e1;
	float averageA = a->verts[0].z + a->verts[1].z + a->verts[2].z / 3;

	triangle_t* b = (triangle_t*)e2;
	float averageB = b->verts[0].z + b->verts[1].z + b->verts[2].z / 3;

	if (averageA < averageB) return -1;
	if (averageA > averageB) return 1;
	return 0;
}

int triangle_clipAgainstPlane(vec3d_t* planePoint, vec3d_t* planeNormal, triangle_t* triangleIn, triangle_t* triangleOut1, triangle_t* triangleOut2) {
	*planeNormal = vec3_normal(planeNormal);

	float distance[3];
	// gets signed distance from planePoint to triangle point
	for (int i = 0; i < 3; i++) {
		vec3d_t v = triangleIn->verts[i];
		v = vec3_add(&v, planePoint);
		distance[i] = vec3_dot(planeNormal, &v);//planePoint) - (planeNormal->x * v.x +planeNormal->y * v.y + planeNormal->z * v.z);
	}

	// create temporary inside and outside arrays used to classify points
	vec3d_t* inside_points[3];  int nInsidePointCount = 0;
	vec3d_t* outside_points[3]; int nOutsidePointCount = 0;

	for (int i = 0; i < 3; i++) {
		// if distance is more than 0, point is 'inside' plane
		if (distance[i] < 0) inside_points[nInsidePointCount++] = &triangleIn->verts[i];
		// oherwise, it is 'outside' plane
		else outside_points[nOutsidePointCount++] = &triangleIn->verts[i];
	}

	if (nInsidePointCount == 0) return 0;

	if (nInsidePointCount == 3) {
		*triangleOut1 = *triangleIn;

//		triangleOut1->color = createColor(0, 0, 255);
		return 1;
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2) {
		triangleOut1->verts[0] = *inside_points[0];

		triangleOut1->verts[1] = vec3_intersectPlane(planePoint, planeNormal, inside_points[0], outside_points[0]);
		triangleOut1->verts[2] = vec3_intersectPlane(planePoint, planeNormal, inside_points[0], outside_points[1]);

		triangleOut1->color = createColor(0, 255, 0);
		return 1;
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1) {
		triangleOut1->verts[0] = *inside_points[0];
		triangleOut1->verts[1] = *inside_points[1];
		triangleOut1->verts[2] = vec3_intersectPlane(planePoint, planeNormal, inside_points[0], outside_points[0]);
		triangleOut1->color = createColor(255, 0, 0);

		triangleOut2->verts[0] = *inside_points[1];
		triangleOut2->verts[1] = triangleOut1->verts[2];
		triangleOut2->verts[2] = vec3_intersectPlane(planePoint, planeNormal, inside_points[1], outside_points[0]);
		triangleOut2->color = createColor(255, 0, 0);


		return 2;
	}
}
