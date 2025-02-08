#ifndef skybox_h
#define skybox_h
#include "shader.h"
typedef struct{
    unsigned int skyboxVAO;
    unsigned int skyboxVBO;
    unsigned int skybox_texture;
    Shader skybox_shader;
}Skybox;


void init_skybox(Skybox *skybox, const char* filepath);
void draw_skybox(Skybox *skybox);
#endif