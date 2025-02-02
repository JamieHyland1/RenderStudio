#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL2\SDL.h>
#include "./Headers/nuklear_container.h"
#include "../include/cglm/cglm.h"
#include "./Headers/shader.h"
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
struct nk_context *ctx;
struct nk_font *font;
int window_width = 640;
int window_height = 480;
bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0; 
float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };
unsigned int cubeVAO,cubeVBO;
float fov = 45.0f;
float aspect_ratio;
mat4 model;
mat4 view;
mat4 projection;
Shader cubeShader;
float a = 0;
///////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
///////////////////////////////////////////////////////////////////////////////
int setup(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL window");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    
    SDL_GetCurrentDisplayMode(0, &displayMode);
    window_width = displayMode.w/2;
    window_height = displayMode.h/2;

    window = SDL_CreateWindow("RenderLab", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        fprintf(stderr, "Error creating SDL window");
        return false;
    }

    context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    glewExperimental = GL_TRUE;
    glewInit();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // Create Nuklear context
    ctx = nk_sdl_init(window);
    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    font = nk_font_atlas_add_default(atlas, 17.0f, NULL);
    nk_sdl_font_stash_end();
    
    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(projection);

    if (!font) {
        fprintf(stderr, "Error: Nuklear font failed to load!\n");
        exit(1);
    }

    nk_style_set_font(ctx, &font->handle);
    return true; 
}


///////////////////////////////////////////////////////////////////////////////
// Initialize various aspects of OpenGL for rendering
///////////////////////////////////////////////////////////////////////////////
bool init_opengl(void) {
    glGenVertexArrays(1,&cubeVAO);
    glGenBuffers(1,&cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    glVertexAttribPointer(0,3,GL_FLOAT,false,3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    cubeShader.fragment_source  = "Shaders/fragment.glsl";
    cubeShader.vertex_source    = "Shaders/vertex.glsl";

    init_shader(&cubeShader);


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
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                }
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function frame by frame with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
    SDL_GetDesktopDisplayMode(0, &displayMode);
    window_width = displayMode.w;
    window_height = displayMode.h;
    int current_time = SDL_GetTicks();
    delta_time = (current_time - previous_frame_time) / 1000.0;
    previous_frame_time = current_time;
    
    glm_mat4_identity(model);
    glm_translate(&model[0], (vec3){0.0,0.0,-5.0});
    glm_rotate(&model[0], (a),(vec3){0.0,1.0,0.0});
    aspect_ratio = (float)window_width / (float)window_height;
    glm_perspective(glm_rad(fov), aspect_ratio, 0.1f, 100.0f, projection);
    a+= 0.01;
    if(a > M_PI)a =0;
    
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(cubeShader.shaderID);
    set_matrix(cubeShader.shaderID,"model",model);
    set_matrix(cubeShader.shaderID,"view",view);
    set_matrix(cubeShader.shaderID,"projection",projection);


    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    

    
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

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* args[]) {
    setup();
    is_running = init_opengl();
    glViewport(0, 0, window_width, window_height);
    while (is_running) {
        process_input();
        update();
        render();
    }
    free_resources();
    return 0;
}