
#include <C:\SDL2\include\SDL2\SDL.h>
#include "../../include/cglm/cglm.h"
#include "../../include/GL/glew.h"

#include "../Headers/shader.h"
#include "../Headers/texture.h"
#include "../Headers/stb_image.h"
#include "../Headers/camera.h"
#include "../Headers/renderer.h"
#include "../Headers/nuklear_container.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>




mat4 cubeModel;
mat4 cubeView;
mat4 cubeProjection;
mat4 quadModel;
mat4 quadView;
mat4 quadProjection;
mat4 lightModel;;
mat4 lightView;
mat4 lightProjection;
Shader cubeShader;
Shader quadShader;
Shader lightShader;
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
float fps;
int window_width = 640;
int window_height = 480;
vec3 p = {0};
int previous_frame_time = 0;
float delta_time = 0; 
float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
vec3 lightColor = {0};
unsigned int cubeVAO,cubeVBO,quadVAO,quadVBO,quadEBO,lightVAO,lightEBO;
vec3 lightPos = {0.0,2.0,5.0};

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
    
    glm_mat4_identity(cubeModel);
    glm_mat4_identity(cubeView);
    glm_mat4_identity(cubeProjection);

    glm_mat4_identity(quadModel);
    glm_mat4_identity(quadView);
    glm_mat4_identity(quadProjection);

    init_camera((vec3){0,1,35},(vec3){0,1,0});

   

    stbi_set_flip_vertically_on_load(1);
    glViewport(0, 0, window_width, window_height);
    return true; 
}
///////////////////////////////////////////////////////////////////////////////
// Initialize various aspects of OpenGL for rendering
///////////////////////////////////////////////////////////////////////////////
bool init_opengl(void) {
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    glGenVertexArrays(1,&lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
  

    cubeShader.vertex_source    = "Shaders/vertex.glsl";
    cubeShader.fragment_source  = "Shaders/fragment.glsl";

    quadShader.vertex_source    = "Shaders/quadVertex.glsl";
    quadShader.fragment_source  = "Shaders/quadFragment.glsl";

    lightShader.vertex_source   = "Shaders/lightVertex.glsl";
    lightShader.fragment_source = "Shaders/lightFragment.glsl";

    init_shader(&cubeShader);
    init_shader(&quadShader);
    init_shader(&lightShader);

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

    glm_mat4_identity(lightModel);
    glm_translate(&lightModel[0], lightPos);
    glm_rotate(&lightModel[0], (a),(vec3){0.0,1.0,0.0});
    camera_look_at(&lightView);
    aspect_ratio = (float)window_width / (float)window_height;
    glm_perspective(glm_rad(fov), aspect_ratio, 0.1f, 100.0f, lightProjection);
    a+= 0.01;
    
    lightPos[2] = 20 * sin(a);
    lightPos[0] = 20 * cos(-a);
    
}
///////////////////////////////////////////////////////////////////////////////
// Render function to draw user interface
///////////////////////////////////////////////////////////////////////////////
void renderUI(){
    static struct nk_colorf color = {1.0f, 0.0f, 0.0f, 1.0f};
    
    if (nk_begin(ctx, "Debug Window", nk_rect(0, 0, 410, 500), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE)) {
        
        // Define row layout with two columns: label + value
        nk_layout_row_dynamic(ctx, 130, 2);
        // Display "Camera Position: "
        nk_label(ctx, "Camera Position:", NK_TEXT_LEFT);
        
        get_camera_position(&p);
        nk_labelf(ctx, NK_TEXT_LEFT, "(%.2f, %.2f, %.2f)", p[0], p[1], p[2]);

        nk_layout_row_dynamic(ctx, 130, 2);
        nk_label(ctx, "Light Colour:", NK_TEXT_LEFT);
        color = nk_color_picker(ctx, color, NK_RGBA); // RGB mode (or NK_RGBA for alpha)
        
        nk_layout_row_dynamic(ctx, 130, 2);
        nk_label(ctx,"Frame Rate: ",NK_TEXT_ALIGN_LEFT);
        nk_labelf(ctx, NK_TEXT_LEFT, "(%.2f)", fps);


        lightColor[0] = color.r;
        lightColor[1] = color.g;
        lightColor[2] = color.b;
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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1.id);
    
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_2D, tex2.id);
    glUseProgram(cubeShader.shaderID);

    set_matrix(cubeShader.shaderID,"model",cubeModel);
    set_matrix(cubeShader.shaderID,"view",cubeView);
    set_matrix(cubeShader.shaderID,"projection",cubeProjection);
    set_vec3(cubeShader.shaderID,"lightColor", lightColor);
    set_vec3(cubeShader.shaderID,"lightPos", lightPos);
    set_vec3(cubeShader.shaderID,"viewPos", p);

    for(int i = -5; i < 5; i ++ ){
        for(int j = -5; j < 5; j ++ ){
            glm_mat4_identity(cubeModel);
            glm_translate(&cubeModel[0], (vec3){1.5 * i,-1.0,-1.5 * j});
            set_matrix(cubeShader.shaderID,"model",cubeModel);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    glUseProgram(lightShader.shaderID);
    set_matrix(lightShader.shaderID,"model",lightModel);
    set_matrix(lightShader.shaderID,"view",lightView);
    set_matrix(lightShader.shaderID,"projection",lightProjection);
    set_vec3(lightShader.shaderID,"lightColor", lightColor);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    // glUseProgram(quadShader.shaderID);
    // set_matrix(quadShader.shaderID,"model",quadModel);
    // set_matrix(quadShader.shaderID,"view",quadView);
    // set_matrix(quadShader.shaderID,"projection",quadProjection);
    // set_int(quadShader.shaderID,"texture1",0);
    // set_int(quadShader.shaderID,"texture2",1);
    

    // glBindVertexArray(quadVAO);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    
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