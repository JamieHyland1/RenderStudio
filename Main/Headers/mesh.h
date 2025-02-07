#ifndef mesh_h
#define mesh_h
#include "vertex.h"
#include "texture.h"
typedef struct{
    Vertex *vertices;
    unsigned int *indices;
    Texture *textures;
    unsigned int VAO;
    unsigned int  VBO;
    unsigned int EBO;
    int numVertices;
    int numIndices;
    int numTextures;

}Mesh;

void init_mesh(Mesh *m);
void draw_mesh(Mesh *m);
#endif