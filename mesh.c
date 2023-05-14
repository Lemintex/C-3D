#include "mesh.h"

mesh_t* CreateCube() {
	mesh_t* cube = (mesh_t*)malloc(sizeof(mesh_t));
	cube->self = cube;
	cube->triangles = (triangle_t*)malloc(sizeof(triangle_t) * 12);
	cube->triangleCount = 12;
		// SOUTH
	cube->triangles[0] = (triangle_t){ 0.0, 0.0, 0.0,    0.0, 1.0, 0.0,    1.0, 1.0, 0.0 };
	cube->triangles[1] = (triangle_t){ 0.0, 0.0, 0.0,    1.0, 1.0, 0.0,    1.0, 0.0, 0.0 };

		// EAST                                                      
	cube->triangles[2] = (triangle_t){ 1.0, 0.0, 0.0,    1.0, 1.0, 0.0,    1.0, 1.0, 1.0 };
	cube->triangles[3] = (triangle_t){ 1.0, 0.0, 0.0,    1.0, 1.0, 1.0,    1.0, 0.0, 1.0 };

		// NORTH                                                    
	cube->triangles[4] = (triangle_t){ 1.0, 0.0, 1.0,    1.0, 1.0, 1.0,    0.0, 1.0, 1.0 };
	cube->triangles[5] = (triangle_t){ 1.0, 0.0, 1.0,    0.0, 1.0, 1.0,    0.0, 0.0, 1.0 };

		// WEST                                                     
	cube->triangles[6] = (triangle_t){ 0.0, 0.0, 1.0,    0.0, 1.0, 1.0,    0.0, 1.0, 0.0 };
	cube->triangles[7] = (triangle_t){ 0.0, 0.0, 1.0,    0.0, 1.0, 0.0,    0.0, 0.0, 0.0 };

		// TOP                                                       
	cube->triangles[8] = (triangle_t){ 0.0, 1.0, 0.0,    0.0, 1.0, 1.0,    1.0, 1.0, 1.0 };
	cube->triangles[9] = (triangle_t){ 0.0, 1.0, 0.0,    1.0, 1.0, 1.0,    1.0, 1.0, 0.0 };

		// BOTTOM                                                    
	cube->triangles[10] = (triangle_t){ 1.0, 0.0, 1.0,    0.0, 0.0, 1.0,    0.0, 0.0, 0.0 };
	cube->triangles[11] = (triangle_t){ 1.0, 0.0, 1.0,    0.0, 0.0, 0.0,    1.0, 0.0, 0.0 };

	return cube;
}

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

	if (averageA < averageB) return 1;
	if (averageA > averageB) return -1;
	return 0;
}
