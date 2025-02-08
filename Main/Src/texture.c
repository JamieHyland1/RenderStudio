#include "../Headers/texture.h"
#include <C:\SDL2\include\SDL2\SDL_opengl.h>
#include "../Headers/stb_image.h"
#include <stdbool.h>




Texture init_texture(const char* filename){
    Texture texture = {0, 0, 0, 0};  // Initialize struct to prevent undefined values

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    // Set texture wrapping & filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Flip images vertically to match OpenGL's coordinate system
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(filename, &texture.width, &texture.height, &texture.nrChannels, 0);
    if (data) {
        GLenum format = (texture.nrChannels == 4) ? GL_RGBA : GL_RGB;  // Auto-detect format
        glTexImage2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, data);
        // glGenerateMipmap(GL_TEXTURE_2D);  // Uncomment if mipmaps are needed
    } else {
        fprintf(stderr, "Failed to load texture: %s\n", filename);
        glDeleteTextures(1, &texture.id);
        texture.id = 0;  // Mark texture as invalid
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);  // Unbind the texture

    return texture;
}
