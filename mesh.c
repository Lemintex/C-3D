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

mesh_t* ReadMeshFromFile(char* file) {


typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    int v1, v2, v3;
} Face;

void read_obj_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }
	mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t));
	//mesh->triangleCount = face_count;
	mesh->triangles = (triangle_t)malloc(0);//sizeof(triangle_t) * mesh->triangleCount;


    vec3d_t* vertices = NULL;
    triangle* faces = NULL;
    int vertex_count = 0;
    int face_count = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            vec3d_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            vertices = (vec3d_t*)realloc(vertices, (vertex_count + 1) * sizeof(vec3d_t));
            vertices[vertex_count++] = vertex;
        } else if (line[0] == 'f' && line[1] == ' ') {
            triangle_t face;
            sscanf(line, "f %d %d %d", &face.verts[0], &face.verts[1], &face.verts[2]);
            faces = (triangle_t*)realloc(faces, (face_count + 1) * sizeof(triangle_t));
            faces[face_count++] = face;
        }
    }

    fclose(file);

    // Process the vertices and faces as needed
    // ...
	mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t));
	mesh->triangleCount = face_count;
	mesh->triangles = (triangle_t)malloc(sizeof(triangle_t) * mesh->triangleCount;

    // Free the allocated memory
    free(vertices);
    free(faces);
}
}
