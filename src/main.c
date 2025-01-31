#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL2\SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include "../include/Nuklear/nuklear.h"
#include "../include/Nuklear/nuklear_sdl_gl3.h"
///////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and game loop
///////////////////////////////////////////////////////////////////////////////
int window_width = 640;
int window_height = 480;
float time = 0;
bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0; 
#define FPS 60
#define FRAME_TARGET_TIME  (1000 / FPS)
static SDL_Window* window = NULL;
SDL_GLContext context = NULL;
static SDL_Renderer* renderer = NULL;
struct nk_context *ctx;
struct nk_font *font;  // Declare a font pointer
struct nk_rect rect;
static float value = 0.6f;
///////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
///////////////////////////////////////////////////////////////////////////////
int setup(void) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL window");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_DisplayMode displayMode;

    SDL_GetCurrentDisplayMode(0, &displayMode);

    // int full_screen_width = displayMode.w;
    // int full_screen_height = displayMode.h;


    //TODO create SDL window
    window = SDL_CreateWindow(
        "RenderLab",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_OPENGL
    );

    if(!window){
        fprintf(stderr, "Error creating SDL window");
        return false;
    }

    //TODO create SDL Renderer
    renderer = SDL_CreateRenderer(window,-1, 0);

    if(!renderer){
        fprintf(stderr, "Error creating SDL renderer");
        return false;
    }

    SDL_SetWindowBordered(window,SDL_WINDOWPOS_CENTERED);
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window,context);
    

    glewExperimental = GL_TRUE;
    glewInit();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    //create nuklear context
    ctx = nk_sdl_init(window);

    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    font = nk_font_atlas_add_default(atlas, 18.0f, NULL);
    nk_sdl_font_stash_end();

    if (!font) {
        fprintf(stderr, "Error: Nuklear font failed to load!\n");
        exit(1);
    }

    nk_style_set_font(ctx, &font->handle);


    
    rect.x= window_width/2;
    rect.y = window_height/2;
    rect.w = 50;
    rect.h = 50;


    return true; 
}

int init_openGL(){

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Poll system events and handle keyboard input
///////////////////////////////////////////////////////////////////////////////
void process_input(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        nk_sdl_handle_event(&event);
        switch (event.type) {
            case SDL_QUIT: {
                is_running = false;
                break;
            }
            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                    break;
                }
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// Update function frame by frame with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
    // Wait some time until the reach the target frame time in milliseconds
    int current_time = SDL_GetTicks();
    delta_time = (current_time - previous_frame_time) / 1000.0;
    previous_frame_time = current_time;
    
    
    previous_frame_time = SDL_GetTicks();
}
///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    nk_sdl_render(NK_ANTI_ALIASING_ON, window_width, window_height);
    SDL_GL_SwapWindow(window);
}




///////////////////////////////////////////////////////////////////////////////
// Free the memory that was dynamically allocated by the program
///////////////////////////////////////////////////////////////////////////////
void free_resources(void) {
    if (context) {
        SDL_GL_DeleteContext(context);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* args[]){
    setup();
    is_running = init_openGL();
    glViewport(0, 0, window_width, window_height);
    while (is_running) {
        process_input();
        update();
        render();
    }

    free_resources();

    return 0;
}
