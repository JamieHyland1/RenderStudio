#include "../Headers/material.h"

void setAmbient(Material *m, vec3 ambient) {
    glm_vec3_copy(ambient, m->ambient);  // Correctly copies the 3 floats
}

void setDiffuse(Material *m, vec3 diffuse){
    glm_vec3_copy(diffuse, m->diffuse);  // Correctly copies the 3 floats
}

void setSpecular(Material *m, vec3 specular){
    glm_vec3_copy(specular, m->specular);  // Correctly copies the 3 floats
}

void setShininess(Material *m, float shininess){
    m->shininess = shininess;
}

void setShader(Material *m, Shader *shader){
    m->shader = shader;
}
