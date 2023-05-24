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

}
