#include <SDL2/SDL.h>
#include "../../include/cglm/cglm.h"
#include "../../include/GL/glew.h"

#include "../Headers/globals.h"
#include "../Headers/stb_image.h"
#include "../Headers/camera.h"
#include "../Headers/renderer.h"
#include "../Headers/UI.h"
#include "../Headers/model.h"

#include <stdio.h>
#include <stdbool.h>
#define FPS 60
#define FRAME_TARGET_TIME  (1000 / FPS)

///////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and game loop
///////////////////////////////////////////////////////////////////////////////
static SDL_Window* window = NULL;
SDL_DisplayMode displayMode;
SDL_GLContext context = NULL;
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

    window = SDL_CreateWindow(
        "RenderLab",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        fprintf(stderr, "Error creating SDL window");
        return false;
    }

    context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(1); // vsync

    glewExperimental = GL_TRUE;
    glewInit();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    stbi_set_flip_vertically_on_load(1);
    glViewport(0, 0, window_width, window_height);

    init_gui(window,context);
    clear_color[0] = 0.2f;
    clear_color[1] = 0.3f;;
    clear_color[2] = 0.3f;
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
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: draw your 3D scene here

    // Draw ImGui on top
    renderUI();

    glEnable(GL_DEPTH_TEST);
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
