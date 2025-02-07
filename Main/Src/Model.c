#include "../Headers/model.h"
#include "../../include/assimp/scene.h"
#include "../../include/assimp/cimport.h"
#include "../../include/assimp/postprocess.h"
#include "../../include/assimp/material.h"

void load_model(Model *model, char *path) {
    printf("Loading model from path: %s\n", path);
    const struct aiScene* scene = aiImportFile(path, aiProcess_Triangulate);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("ERROR::ASSIMP::%s\n", aiGetErrorString());
        return;
    }

    printf("Model loaded successfully. Number of meshes in scene: %d\n", scene->mNumMeshes);
    
    char* directory = strdup(path);
    char* last_slash = strrchr(directory, '/');
    if (last_slash != NULL) {
        last_slash[1] = '\0';
    }
    model->path = strdup(directory);
    model->numMeshes = scene->mNumMeshes;
    model->meshes = malloc(sizeof(Mesh) * model->numMeshes);
    
    process_node(model, scene->mRootNode, scene);
    printf("Finished processing model. Total meshes processed: %d\n", model->numMeshes);
    for(int i = 0; i < model->numMeshes; i ++){
        init_mesh(&model->meshes[i]);
    }
}

void process_node(Model *model, struct aiNode* node, const struct aiScene* scene) {
    printf("Processing node: %s, Number of meshes: %d\n", node->mName.data, node->mNumMeshes);
    
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        unsigned int meshIndex = node->mMeshes[i];
        printf("Processing mesh index: %d\n", meshIndex);
        struct aiMesh* aimesh = scene->mMeshes[meshIndex];
        Mesh mesh;
        memset(&mesh, 0, sizeof(Mesh));
        process_mesh(&mesh, aimesh, scene, model->path);
        model->meshes[i] = mesh;
    }
    
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        printf("Processing child node: %d\n", i);
        process_node(model, node->mChildren[i], scene);
    }
}

void process_mesh(Mesh *mesh, struct aiMesh* aimesh, const struct aiScene* scene, char* directory) {
    printf("Processing mesh with %d vertices and %d faces\n", aimesh->mNumVertices, aimesh->mNumFaces);
    
    mesh->numVertices = aimesh->mNumVertices;
    mesh->vertices = calloc(mesh->numVertices, sizeof(Vertex));
    for (unsigned int i = 0; i < mesh->numVertices; i++) {
        Vertex vertex;
        memset(&vertex, 0, sizeof(Vertex));
        vertex.position[0] = aimesh->mVertices[i].x;
        vertex.position[1] = aimesh->mVertices[i].y;
        vertex.position[2] = aimesh->mVertices[i].z;
        
        if (aimesh->mNormals) {
            vertex.normal[0] = aimesh->mNormals[i].x;
            vertex.normal[1] = aimesh->mNormals[i].y;
            vertex.normal[2] = aimesh->mNormals[i].z;
        }
        
        if (aimesh->mTextureCoords[0]) {
            vertex.texcoord[0] = aimesh->mTextureCoords[0][i].x;
            vertex.texcoord[1] = aimesh->mTextureCoords[0][i].y;
        }
        
        mesh->vertices[i] = vertex;
    }
    
    mesh->numIndices = 0;
    for (unsigned int i = 0; i < aimesh->mNumFaces; i++) {
        mesh->numIndices += aimesh->mFaces[i].mNumIndices;
    }
    mesh->indices = malloc(sizeof(unsigned int) * mesh->numIndices);
    printf("Mesh has %d indices\n", mesh->numIndices);
    
    int indexOffset = 0;
    for (unsigned int i = 0; i < aimesh->mNumFaces; i++) {
        struct aiFace face = aimesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            mesh->indices[indexOffset++] = face.mIndices[j];
        }
    }
    
    if (aimesh->mMaterialIndex >= 0) {
        struct aiMaterial *material = scene->mMaterials[aimesh->mMaterialIndex];
        load_material_textures(mesh, material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
        load_material_textures(mesh, material, aiTextureType_SPECULAR, "texture_specular", directory);
    }
}

void load_material_textures(Mesh* mesh, struct aiMaterial* mat, enum aiTextureType type, char* typeName, char* directory) {
    mesh->numTextures = aiGetMaterialTextureCount(mat, type);
    printf("Loading %d textures of type %s\n", mesh->numTextures, typeName);
    mesh->textures = malloc(sizeof(Texture) * mesh->numTextures);
    int textureIndex = 0;
    for (unsigned int i = 0; i < mesh->numTextures; i++) {
        struct aiString str;
        enum aiReturn rtrn = aiGetMaterialTexture(mat, type, i, &str, NULL, NULL, NULL, NULL, NULL, NULL);
        if (rtrn == aiReturn_SUCCESS) {
            printf("Texture path: %s\n", str.data);
            Texture texture = init_texture(str.data);
            texture.type = typeName;
            mesh->textures[textureIndex++] = texture;
        } else if (rtrn == aiReturn_FAILURE) {
            printf("Error loading texture\n");
        }
    }
}
void draw_model(Model *model){
    for(int i = 0; i < model->numMeshes; i++){
        draw_mesh(&model->meshes[i]);
    }
}
