#ifndef material_h
#define material_h
#include <C:\SDL2\include\SDL2\SDL.h>
#include "../../include/cglm/cglm.h"
#include "../Headers/shader.h"
typedef struct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    Shader *shader;
}Material;

void setAmbient(Material *m, vec3 ambient);

void setDiffuse(Material *m, vec3 diffuse);

void setSpecular(Material *m, vec3 specular);

void setShininess(Material *m, float shininess);

void setShader(Material *m, Shader *shader);


#endif