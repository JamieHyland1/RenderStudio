#include "../../include/GL/glew.h"
#include "../Headers/shader.h"

//Initialize vertex and fragment shaders and link them to the shader program
void init_shader(Shader *shader) {
    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    // Load vertex shader source
    const GLchar *vertexSource = get_shader_source(shader->vertex_source);
    if (!vertexSource) {
        printf("ERROR::SHADER::VERTEX::FAILED TO LOAD FILE: %s\n", shader->vertex_source);
        return;
    }
    
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);  

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);   
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Load fragment shader source
    const GLchar *fragmentSource = get_shader_source(shader->fragment_source);
    if (!fragmentSource) {
        printf("ERROR::SHADER::FRAGMENT::FAILED TO LOAD FILE: %s\n", shader->fragment_source);
        free((void*)vertexSource); // Free vertex source if fragment load fails
        return;
    }

    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);   
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
    }

    shader->shaderID = glCreateProgram();

    glAttachShader(shader->shaderID, vertexShader);
    glAttachShader(shader->shaderID, fragmentShader);
    glLinkProgram(shader->shaderID);

    glGetProgramiv(shader->shaderID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader->shaderID, 512, NULL, infoLog);
        printf("ERROR::LINKING::SHADER::COMPILATION_FAILED: %s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free((void*)vertexSource);
    free((void*)fragmentSource);
}

//Read the shader files and return a buffer containing shader logic
GLchar* get_shader_source(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error opening shader file: %s\n", filename);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    GLchar* buffer = (GLchar*)malloc(size + 1);
    if (!buffer) {
        printf("Memory allocation failed for shader file: %s\n", filename);
        fclose(fp);
        return NULL;
    }

    fread(buffer, 1, size, fp);
    buffer[size] = '\0';  // Null-terminate the shader source

    fclose(fp);
    return buffer;
}


//Set various uniformes in shader

void set_bool  (int id, char* name, bool value){
    glUniform1i(glGetUniformLocation(id,name),value);
}

void set_int   (int id, char* name, int value){
    glUniform1i(glGetUniformLocation(id, name), value);
}

void set_float (int id, char* name, float value){
    glUniform1f(glGetUniformLocation(id,name),value);
}

void set_vec3 (int id, char* name, vec3 color){
    glUniform3f(glGetUniformLocation(id, name),color[0],color[1],color[2]);
}

void set_matrix(int id, char* name, mat4 mat){
    glUniformMatrix4fv(glGetUniformLocation(id, name),1,GL_FALSE,(float *)mat);
}