#ifndef shader_h
#define shader_h
#include <C:\SDL2\include\SDL2\SDL.h>
#include <C:\SDL2\include\SDL2\SDL_opengl.h>
#include "../../include/cglm/cglm.h"
typedef struct {
    int shaderID;
    char *fragment_source;
    char *vertex_source;


}Shader;


void init_shader(Shader *shader);
GLchar* get_shader_source(const char* filename);
void set_bool   (int id, char* name, bool value);
void set_int    (int id, char* name, int value);
void set_float  (int id, char* name, float value);
void set_vec3   (int id, char* name, vec3 color);
void set_matrix (int id, char* name, mat4 mat);
#endif