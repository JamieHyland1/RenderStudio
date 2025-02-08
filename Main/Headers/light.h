#ifndef light_h
#define light_h
#include "../../include/cglm/cglm.h"
typedef struct{
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}Light;

#endif