#include "../../include/GL/glew.h"
#include "../Headers/nuklear_container.h"
#include <C:\SDL2\include\SDL2\SDL.h>
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION

#include "../../include/Nuklear/nuklear.h"
#include "../../include/Nuklear/nuklear_sdl_gl3.h"
#include <stdbool.h>
struct nk_context *ctx;
struct nk_font *font;

bool initNuklear(SDL_Window* window){
    ctx = nk_sdl_init(window);
    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    font = nk_font_atlas_add_default(atlas, 17.0f, NULL);
    nk_sdl_font_stash_end();

     if (!font) {
        fprintf(stderr, "Error: Nuklear font failed to load!\n");
        return false;
    }

   nk_style_set_font(ctx, &font->handle);
   return true;
}