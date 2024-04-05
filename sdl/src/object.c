#include "object.h"
#include <stdio.h>
#include <stdlib.h>

void object_attach_shaders(object_t *obj, const char *vertex_shader_path, const char *fragment_shader_path){
    GLuint vertex_shader = load_and_compile_shader(vertex_shader_path, VERTEX);
    GLuint fragment_shader = load_and_compile_shader(fragment_shader_path, FRAGMENT);
    obj->shader_program = glCreateProgram();
    glAttachShader(obj->shader_program, vertex_shader);
    glAttachShader(obj->shader_program, fragment_shader);
    glLinkProgram(obj->shader_program);

    int success;
    char infoLog[512];
    glGetProgramiv(obj->shader_program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(obj->shader_program, 512, NULL, infoLog);
        printf("Error! Program linking failed: %s\n", infoLog);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void object_render(object_t *obj) {
    glBindVertexArray(obj->vao);
    glDrawElements(GL_TRIANGLES, obj->num_vertices, GL_UNSIGNED_INT, 0);
}

void object_use(object_t *obj) {
    glUseProgram(obj->shader_program);
}

void object_gen_buffers(object_t *obj) {
    glGenVertexArrays(1, &(obj->vao));
    glGenBuffers(1, &(obj->vbo));
    glGenBuffers(1, &(obj->ebo));
}

void object_bind_buffers(object_t *obj) {
    glBindVertexArray(obj->vao);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->ebo);
}

void object_gen_textures(object_t *obj, unsigned int num_textures) {
    obj->textures = malloc(num_textures * sizeof(GLuint));
    glGenTextures(num_textures, obj->textures);
}

void object_bind_texture(object_t *obj, unsigned int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, obj->textures[index]);
}

void object_free(object_t *obj) {
    free(obj->textures);
}