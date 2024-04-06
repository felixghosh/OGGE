#ifndef OBJECT_H
#define OBJECT_H

#include "gl_utils.h"

typedef struct object {
    GLfloat *vertices;
    GLuint *indices;
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    GLuint shader_program;
    unsigned int num_vertices;
} object;

void object_attach_shaders(object *obj, const char *vertex_shader_path, const char *fragment_shader_path);

void object_render(object *obj);

void object_generate_buffers(object *obj);

void object_bind_buffers(object *obj);

#endif