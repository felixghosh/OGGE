#ifndef OBJECT_H
#define OBJECT_H

#include "gl_utils.h"

typedef struct object_t {
    GLfloat *vertices;
    GLuint *indices;
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    GLuint shader_program;
    unsigned int num_vertices;
    GLuint texture;
} object_t;

void object_attach_shaders(object_t *obj, const char *vertex_shader_path, const char *fragment_shader_path);

void object_render(object_t *obj);

void object_use(object_t *obj);

void object_gen_buffers(object_t *obj);

void object_bind_buffers(object_t *obj);

void object_gen_texture(object_t *obj);

void object_bind_texture(object_t *obj);

#endif