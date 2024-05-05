#include "object.h"
#include <stdio.h>
#include <stdlib.h>

#include "transform.h"

void object_attach_shaders(object *obj, const char *vertex_shader_path, const char *fragment_shader_path){
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

void object_render(object *obj) {
    glBindVertexArray(obj->vao);
    glDrawElements(GL_TRIANGLES, obj->num_vertices, GL_UNSIGNED_INT, 0);
}

void object_use(object *obj) {
    glUseProgram(obj->shader_program);
}

void object_gen_buffers(object *obj) {
    glGenVertexArrays(1, &(obj->vao));
    glGenBuffers(1, &(obj->vbo));
    glGenBuffers(1, &(obj->ebo));
}

void object_bind_buffers(object *obj) {
    glBindVertexArray(obj->vao);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->ebo);
}

void object_gen_textures(object *obj, unsigned int num_textures) {
    obj->textures = malloc(num_textures * sizeof(GLuint));
    glGenTextures(num_textures, obj->textures);
}

void object_bind_texture(object *obj, unsigned int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, obj->textures[index]);
}

void object_free(object *obj) {
    free(obj->textures);
}

mat4 object_model_mat(object *obj){
    mat4 model = mat4_identity();
    model = mat4_mul(model, transform_translate(obj->pos.v[0], obj->pos.v[1], obj->pos.v[2]));
    model = mat4_mul(model, transform_scale(obj->scale, obj->scale, obj->scale));
    model = mat4_mul(model, transform_rotate_x(obj->orientation.v[0]));
    model = mat4_mul(model, transform_rotate_y(obj->orientation.v[1]));
    model = mat4_mul(model, transform_rotate_z(obj->orientation.v[2]));
    return model;
}