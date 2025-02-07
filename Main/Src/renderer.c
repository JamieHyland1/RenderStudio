
#include <C:\SDL2\include\SDL2\SDL.h>
#include "../../include/cglm/cglm.h"
#include "../../include/GL/glew.h"

#include "../Headers/shader.h"
#include "../Headers/texture.h"
#include "../Headers/stb_image.h"
#include "../Headers/camera.h"
#include "../Headers/renderer.h"
#include "../Headers/material.h"
#include "../Headers/model.h"
#include "../Headers/nuklear_container.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>



#define FPS 60
#define FRAME_TARGET_TIME  (1000 / FPS)

///////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and game loop
///////////////////////////////////////////////////////////////////////////////
static SDL_Window* window = NULL;
SDL_DisplayMode displayMode;
SDL_GLContext context = NULL;
float fps;
int window_width = 640;
int window_height = 480;
int previous_frame_time = 0;
float delta_time = 0; 
float aspect_ratio;
Model model;

///////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
///////////////////////////////////////////////////////////////////////////////
int setup_renderer(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL window");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    
    SDL_GetCurrentDisplayMode(0, &displayMode);
    window_width = displayMode.w;
    window_height = displayMode.h;

    window = SDL_CreateWindow("RenderLab", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        fprintf(stderr, "Error creating SDL window");
        return false;
    }
    context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    glewExperimental = GL_TRUE;
    glewInit();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    initNuklear(window);



    stbi_set_flip_vertically_on_load(1);
    glViewport(0, 0, window_width, window_height);
    return true; 
}
///////////////////////////////////////////////////////////////////////////////
// Initialize various aspects of OpenGL for rendering
///////////////////////////////////////////////////////////////////////////////
bool init_opengl(void) {
    //TODO: ADD initialization code for models
    return true;
}
///////////////////////////////////////////////////////////////////////////////
// Update function frame by frame with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
    SDL_GetDesktopDisplayMode(0, &displayMode);
    // Frame timing
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    // Compute delta time (frame duration in seconds)
    Uint32 current_time = SDL_GetTicks();
    delta_time = (current_time - previous_frame_time) / 1000.0f;
    previous_frame_time = current_time; // Update for next frame

    // Calculate FPS
    fps = (delta_time > 0) ? (1.0f / delta_time) : 0.0f;
    process_keyboard_movement(delta_time);

   
}
///////////////////////////////////////////////////////////////////////////////
// Render function to draw user interface
///////////////////////////////////////////////////////////////////////////////
void renderUI(){
    static struct nk_colorf color = {1.0f, 1.0f, 1.0f, 1.0f};
    
    if (nk_begin(ctx, "Debug Window", nk_rect(0, 0, 410, 500), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE)) {
        
        nk_layout_row_dynamic(ctx, 30, 2);
        nk_label(ctx,"Frame Rate: ",NK_TEXT_ALIGN_LEFT);
        nk_labelf(ctx, NK_TEXT_LEFT, "(%.2f)", fps);
        // Define row layout with two columns: label + value
        nk_layout_row_dynamic(ctx, 130, 2);
        // Display "Camera Position: "
        nk_label(ctx, "Camera Position:", NK_TEXT_LEFT);
        Vec3 p = {0};
        get_camera_position(&p);
        nk_labelf(ctx, NK_TEXT_LEFT, "(%.2f, %.2f, %.2f)", p[0], p[1], p[2]);

        nk_layout_row_dynamic(ctx, 130, 2);
        nk_label(ctx, "Light Colour:", NK_TEXT_LEFT);
        color = nk_color_picker(ctx, color, NK_RGBA); // RGB mode (or NK_RGBA for alpha)
        
    
    }
    nk_end(ctx);
}
///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderUI();
    glEnable(GL_DEPTH_TEST);



    




    
    nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
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