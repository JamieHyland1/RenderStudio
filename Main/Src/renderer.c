#include <C:\SDL2\include\SDL2\SDL.h>
#include "../../include/cglm/cglm.h"
#include "../../include/GL/glew.h"

#include "../Headers/shader.h"
#include "../Headers/texture.h"
#include "../Headers/stb_image.h"
#include "../Headers/camera.h"
#include "../Headers/renderer.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>




mat4 cubeModel;
mat4 cubeView;
mat4 cubeProjection;
mat4 quadModel;
mat4 quadView;
mat4 quadProjection;
Shader cubeShader;
Shader quadShader;
Texture tex1;
Texture tex2;

#define FPS 60
#define FRAME_TARGET_TIME  (1000 / FPS)

///////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and game loop
///////////////////////////////////////////////////////////////////////////////
static SDL_Window* window = NULL;
SDL_DisplayMode displayMode;
SDL_GLContext context = NULL;
// struct nk_context *ctx;
// struct nk_font *font;
int window_width = 640;
int window_height = 480;

int previous_frame_time = 0;
float delta_time = 0; 
float cubeVertices[] = {
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
float quadVertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};
unsigned int quadIndices[] = {  
0, 1, 3, // first triangle
1, 2, 3  // second triangle
};


unsigned int cubeVAO,cubeVBO,quadVAO,quadVBO,quadEBO;


float aspect_ratio;

float a = 0;
///////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
///////////////////////////////////////////////////////////////////////////////
int setup_renderer(void) {
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
    
    //TODO: Create Nuklear context - Get back to this later
    ////////////////////////////////////////////////////////
    // ctx = nk_sdl_init(window);
    // struct nk_font_atlas *atlas;
    // nk_sdl_font_stash_begin(&atlas);
    // font = nk_font_atlas_add_default(atlas, 17.0f, NULL);
    // nk_sdl_font_stash_end();
    //////////////////////////////////////////////////////////
    
    glm_mat4_identity(cubeModel);
    glm_mat4_identity(cubeView);
    glm_mat4_identity(cubeProjection);

    glm_mat4_identity(quadModel);
    glm_mat4_identity(quadView);
    glm_mat4_identity(quadProjection);

    init_camera((vec3){0},(vec3){0,1,0});

    // if (!font) {
    //     fprintf(stderr, "Error: Nuklear font failed to load!\n");
    //     exit(1);
    // }

   // nk_style_set_font(ctx, &font->handle);

    stbi_set_flip_vertically_on_load(1);
    glViewport(0, 0, window_width, window_height);
    return true; 
}

///////////////////////////////////////////////////////////////////////////////
// Initialize various aspects of OpenGL for rendering
///////////////////////////////////////////////////////////////////////////////
bool init_opengl(void) {
    glGenVertexArrays(1,&cubeVAO);
    glGenBuffers(1,&cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    glVertexAttribPointer(0,3,GL_FLOAT,false,3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    cubeShader.vertex_source    = "Shaders/vertex.glsl";
    cubeShader.fragment_source  = "Shaders/fragment.glsl";

    quadShader.vertex_source    = "Shaders/quadVertex.glsl";
    quadShader.fragment_source  = "Shaders/quadFragment.glsl";

    init_shader(&cubeShader);
    init_shader(&quadShader);

    tex1 = init_texture("Assets/Textures/container.jpg");
    tex2 = init_texture("Assets/Textures/gear5.jpg");


   return true;
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
    process_keyboard_movement(delta_time);

    
    glm_mat4_identity(cubeModel);
    //glm_translate(&cubeModel[0], (vec3){0.0,0.0,-5.0});
    glm_rotate(&cubeModel[0], (a),(vec3){0.0,1.0,0.0});
    aspect_ratio = (float)window_width / (float)window_height;
    camera_look_at(&cubeView);
    glm_perspective(glm_rad(fov), aspect_ratio, 0.1f, 100.0f, cubeProjection);

    glm_mat4_identity(quadModel);
    glm_translate(&quadModel[0], (vec3){-3.0,0.0,5.0});
    //glm_rotate(&quadModel[0], (a),(vec3){0.0,1.0,0.0});
    camera_look_at(&quadView);
    aspect_ratio = (float)window_width / (float)window_height;
    glm_perspective(glm_rad(fov), aspect_ratio, 0.1f, 100.0f, quadProjection);
    a+= 0.01;
    if(a > M_PI)a =0;
    
}


///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1.id);
    
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_2D, tex2.id);
    glUseProgram(cubeShader.shaderID);

    set_matrix(cubeShader.shaderID,"model",cubeModel);
    set_matrix(cubeShader.shaderID,"view",cubeView);
    set_matrix(cubeShader.shaderID,"projection",cubeProjection);

    for(int i = 0; i < 10; i ++ ){
        for(int j = 0; j < 10; j ++ ){
            glm_mat4_identity(cubeModel);
            glm_translate(&cubeModel[0], (vec3){1.5 * i,-1.0,-1.5 * j});
            set_matrix(cubeShader.shaderID,"model",cubeModel);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }


    glUseProgram(quadShader.shaderID);
    set_matrix(quadShader.shaderID,"model",quadModel);
    set_matrix(quadShader.shaderID,"view",quadView);
    set_matrix(quadShader.shaderID,"projection",quadProjection);
    set_int(quadShader.shaderID,"texture1",0);
    set_int(quadShader.shaderID,"texture2",1);
    

    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    
    //nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
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