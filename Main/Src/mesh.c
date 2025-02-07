
#include <C:\SDL2\include\SDL2\SDL.h>
#include "../../include/cglm/cglm.h"
#include "../../include/GL/glew.h"
#include "../Headers/mesh.h"


void init_mesh(Mesh *m){
    glGenVertexArrays(1, &m->VAO);
    glGenBuffers(1, &m->VBO);
    glGenBuffers(1, &m->EBO);
  
    glBindVertexArray(m->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m->VBO);
   
    glBufferData(GL_ARRAY_BUFFER, m->numVertices * sizeof(Vertex), &m->vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->EBO);
   
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,  m->numIndices * sizeof(unsigned int),&m->indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

    glBindVertexArray(0);
}

void draw_mesh(Mesh *m){
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    
    // for(unsigned int i = 0; i < m->numTextures; i++)
    // {
    //     glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
    //     // retrieve texture number (the N in diffuse_textureN)
    //     char *number;
    //     char *name = m->textures[i].type;
    //     if(name == "texture_diffuse"){
    //       //  number = std::to_string(diffuseNr++);
    //     }
    //     else if(name == "texture_specular"){
    //       //  number = std::to_string(specularNr++);
    //     }

    //     //shader.setInt(("material." + name + number).c_str(), i);
    //     glBindTexture(GL_TEXTURE_2D, m->textures[i].id);
    // }
    // glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(m->VAO);
    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}