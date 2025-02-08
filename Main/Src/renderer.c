
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
#include "../Headers/light.h"
#include "../Headers/skybox.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define NUM_LIGHTS 5
#define NUM_MODELS 9
#define FPS 60
#define FRAME_TARGET_TIME  (1000 / FPS)

Light lights[NUM_LIGHTS];
Model models[NUM_MODELS];

Skybox skybox;

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
Material material;

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

    window = SDL_CreateWindow("RenderLab", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);

    if (!window) {
        fprintf(stderr, "Error creating SDL window");
        return false;
    }
    context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    glewExperimental = GL_TRUE;
    glewInit();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if(!initNuklear(window)){
        fprintf(stderr, "Error intialising nuklear UI");
        return false;
    }
    init_camera((vec3){2.5,0,25}, (vec3){0,1,0});
    init_skybox(&skybox, "Assets/Textures/skybox/");

    stbi_set_flip_vertically_on_load(1);
    glViewport(0, 0, window_width, window_height);
    return true; 
}
///////////////////////////////////////////////////////////////////////////////
// Initialize various aspects of OpenGL for rendering
///////////////////////////////////////////////////////////////////////////////
bool init_opengl(void) {
    //TODO: ADD initialization code for models
    int grid_size = (int)sqrt(NUM_MODELS);  // Compute the grid size (e.g., 3 for 9 models)
    float spacing = 2.5f; // Distance between cubes
    
    for (int i = 0; i < NUM_MODELS; i++) {
        int row = i / grid_size;  // Compute row index
        int col = i % grid_size;  // Compute column index
    
        load_model(&models[i], "Assets/Models/Cube/cube.obj");
    
        models[i].material.shader = malloc(sizeof(Shader)); // Allocate shader
        models[i].material.shader->vertex_source = "Shaders/vertex.glsl";
        models[i].material.shader->fragment_source = "Shaders/fragment.glsl";
        init_shader(models[i].material.shader);
    
        // Set positions in a grid pattern
        models[i].position[0] = col * spacing;  // x-axis (column)
        models[i].position[1] = 0;              // y-axis (constant height)
        models[i].position[2] = -row * spacing; // z-axis (row)
    }
    
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
    aspect_ratio = (float)window_width / (float)window_height;
    glm_perspective(glm_rad(fov), aspect_ratio, 0.1f, 100.0f, camera.projection);
    camera_look_at();
    for(int i = 0; i < NUM_MODELS; i++){
        update_model(&models[i]);
    }
   
}
///////////////////////////////////////////////////////////////////////////////
// Render function to draw user interface
///////////////////////////////////////////////////////////////////////////////
void renderUI(){
    static struct nk_colorf color = {1.0f, 1.0f, 1.0f, 1.0f};
    
    if (nk_begin(ctx, "Debug Window", nk_rect(0, 0, 500, window_height), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_SCALABLE)) {
        nk_layout_row_dynamic(ctx, 15, 2);
        // Display "Camera Position: "
        nk_label(ctx, "Camera Position:", NK_TEXT_LEFT);
        vec3 p = {0};
        get_camera_position(&p);
        nk_labelf(ctx, NK_TEXT_LEFT, "(%.2f, %.2f, %.2f)", p[0], p[1], p[2]);
        nk_layout_row_dynamic(ctx, 15, 2);
        // Display "Camera Position: "
        nk_label(ctx, "Number of Models: ", NK_TEXT_LEFT);
        nk_labelf(ctx, NK_TEXT_LEFT, "(%d)", NUM_MODELS);
        nk_label(ctx, "Number of Lights: ", NK_TEXT_LEFT);
        nk_labelf(ctx, NK_TEXT_LEFT, "(%d)", NUM_LIGHTS);
        for(int i = 0; i < NUM_MODELS; i ++){
            nk_layout_row_dynamic(ctx, 15, 2);
            nk_label(ctx, "Object Position", NK_TEXT_LEFT);
            vec3 model_pos;
            glm_vec3_copy(models[i].position,model_pos);
            nk_labelf(ctx, NK_TEXT_LEFT, "[%d]: (%.2f, %.2f, %.2f)",i, model_pos[0], model_pos[1], model_pos[2]);
        }

        for(int i = 0; i < NUM_MODELS; i ++){
            nk_layout_row_dynamic(ctx, 15, 2);
            nk_label(ctx, "Object Rotation", NK_TEXT_LEFT);
            vec3 model_rot;
            glm_vec3_copy(models[i].rotation,model_rot);
            nk_labelf(ctx, NK_TEXT_LEFT, "[%d]: (%.2f, %.2f, %.2f)",i, model_rot[0], model_rot[1], model_rot[2]);
        }

        for(int i = 0; i < NUM_MODELS; i ++){
            nk_layout_row_dynamic(ctx, 15, 2);
            nk_label(ctx, "Object Scale", NK_TEXT_LEFT);
            vec3 model_scale;
            glm_vec3_copy(models[i].scale,model_scale);
            nk_labelf(ctx, NK_TEXT_LEFT, "[%d]: (%.2f, %.2f, %.2f)",i, model_scale[0], model_scale[1], model_scale[2]);
        }

    }
    nk_end(ctx);

    // if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
    //         NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
    //         NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
    //     {
    //         enum {EASY, HARD};
    //         static int op = EASY;
    //         static int property = 20;

    //         nk_layout_row_static(ctx, 30, 80, 1);
    //         if (nk_button_label(ctx, "button"))
    //             fprintf(stdout, "button pressed\n");
    //         nk_layout_row_dynamic(ctx, 30, 2);
    //         if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
    //         if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
    //         nk_layout_row_dynamic(ctx, 25, 1);
    //         nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

    //         nk_layout_row_dynamic(ctx, 20, 1);
    //         nk_label(ctx, "background:", NK_TEXT_LEFT);
    //         nk_layout_row_dynamic(ctx, 25, 1);
    //         if (nk_combo_begin_color(ctx, nk_rgb_cf(color), nk_vec2(nk_widget_width(ctx),400))) {
    //             nk_layout_row_dynamic(ctx, 120, 1);
    //             color = nk_color_picker(ctx, color, NK_RGBA);
    //             nk_layout_row_dynamic(ctx, 25, 1);
    //             color.r = nk_propertyf(ctx, "#R:", 0, color.r, 1.0f, 0.01f,0.005f);
    //             color.g = nk_propertyf(ctx, "#G:", 0, color.g, 1.0f, 0.01f,0.005f);
    //             color.b = nk_propertyf(ctx, "#B:", 0, color.b, 1.0f, 0.01f,0.005f);
    //             color.a = nk_propertyf(ctx, "#A:", 0, color.a, 1.0f, 0.01f,0.005f);
    //             nk_combo_end(ctx);
    //         }
    //     }
    //     nk_end(ctx);

    if (nk_begin(ctx, "Demo", nk_rect(50, 50, 200, 200),
        NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
        NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
    {
        nk_menubar_begin(ctx);
        nk_layout_row_begin(ctx, NK_STATIC, 25, 2);
        nk_layout_row_push(ctx, 45);
        if (nk_menu_begin_label(ctx, "FILE", NK_TEXT_LEFT, nk_vec2(120, 200))) {
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_menu_item_label(ctx, "OPEN", NK_TEXT_LEFT);
            nk_menu_item_label(ctx, "CLOSE", NK_TEXT_LEFT);
            nk_menu_end(ctx);
        }
        nk_layout_row_push(ctx, 45);
        if (nk_menu_begin_label(ctx, "EDIT", NK_TEXT_LEFT, nk_vec2(120, 200))) {
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_menu_item_label(ctx, "COPY", NK_TEXT_LEFT);
            nk_menu_item_label(ctx, "CUT", NK_TEXT_LEFT);
            nk_menu_item_label(ctx, "PASTE", NK_TEXT_LEFT);
            nk_menu_end(ctx);
        }
        nk_layout_row_end(ctx);
        nk_menubar_end(ctx);

        {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;
            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "button"))
                fprintf(stdout, "button pressed\n");
             nk_layout_row_dynamic(ctx, 30, 2);
             if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
             if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
             nk_layout_row_dynamic(ctx, 25, 1);
             nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
        }
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
    // Draw models
    for(int i = 0; i < NUM_MODELS; i++){
        draw_model(&models[i]);
    }

    draw_skybox(&skybox);

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
    for(int i = 0; i < NUM_MODELS; i++){
        Model model = models[i];
        free_model(&model);
    }
    SDL_Quit();
}