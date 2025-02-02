#ifndef texture_h
#define texture_h
#include <C:\SDL2\include\SDL2\SDL_opengl.h>
typedef struct{
    GLuint id;
    int width;
    int height;
    int nrChannels;
    unsigned char *data;
    char* type;
}Texture;

Texture init_texture(const char* filename);
#endif