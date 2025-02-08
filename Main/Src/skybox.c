#include "../../include/GL/glew.h"
#include <C:\SDL2\include\SDL2\SDL.h>
#include <C:\SDL2\include\SDL2\SDL_opengl.h>
#include "../../include/cglm/cglm.h"

#include "../Headers/stb_image.h"
#include "../Headers/skybox.h"
#include "../Headers/camera.h"
#include "../Headers/shader.h"
#define NUM_FACES 6
static const char* faces[] = {
    "right.jpg",
    "left.jpg",
    "top.jpg",
    "bottom.jpg",
    "front.jpg",
    "back.jpg"
};
static float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
void init_skybox(Skybox *skybox, const char* filename){
    
    glGenTextures(1, &skybox->skybox_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->skybox_texture);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < NUM_FACES; i++) {
        char* path_to_image = malloc(strlen(filename) + strlen(faces[i]) + 1);
        if (!path_to_image) {
            printf("Failed to allocate memory for path_to_image\n");
            continue;
        }
        strcpy(path_to_image, filename);
        strcat(path_to_image, faces[i]);

        unsigned char *data = stbi_load(path_to_image, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            printf("Cubemap texture failed to load at path: %s\n", path_to_image);
        }

        free(path_to_image);
    }

   
    skybox->skybox_shader.vertex_source     = "Shaders/skybox_vertex.glsl";
    skybox->skybox_shader.fragment_source   = "Shaders/skybox_fragment.glsl";

    init_shader(&skybox->skybox_shader);


    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    
    glGenVertexArrays(1, &skybox->skyboxVAO);
    glGenBuffers(1, &skybox->skyboxVBO);
    glBindVertexArray(skybox->skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skybox->skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    
}
void draw_skybox(Skybox *skybox){
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    glUseProgram(skybox->skybox_shader.shaderID);
    mat4 view; 

    //Remove the translation component from the view matrix so the cube draws on the origin
    glm_mat4_copy(camera.view,view);

    view[3][0] = 0.0f;
    view[3][1] = 0.0f;
    view[3][2] = 0.0f;
    view[3][3] = 1.0f; 

    set_matrix(skybox->skybox_shader.shaderID, "view", view);
    set_matrix(skybox->skybox_shader.shaderID, "projection", camera.projection);
    // skybox cube
    glBindVertexArray(skybox->skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->skybox_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}