#ifndef NUKLEAR_CONTAINER_H
#define NUKLEAR_CONTAINER_H

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

// Nuklear feature flags (keep them consistent across all files)
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include "../../include/Nuklear/nuklear.h"
#include "../../include/Nuklear/nuklear_sdl_gl3.h"
extern struct nk_context *ctx;
void initNuklear(SDL_Window* window);

#endif // NUKLEAR_CONTAINER_H
