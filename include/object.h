#ifndef OBJECT_H
#define OBJECT_H

#include "gl_utils.h"
#include "vec.h"
#include "mat.h"

typedef struct object {
    GLfloat *vertices;
    GLuint *indices;
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    GLuint shader_program;
    unsigned int num_vertices;
    GLuint *textures;
    vec3 pos;
    float scale;
    vec3 orientation;
} object;

void object_attach_shaders(object *obj, const char *vertex_shader_path, const char *fragment_shader_path);

void object_render(object *obj);

void object_use(object *obj);

void object_gen_buffers(object *obj);

void object_bind_buffers(object *obj);

void object_gen_textures(object *obj, unsigned int num_textures);

void object_bind_texture(object *obj, unsigned int index);

void object_free(object *obj);

mat4 object_model_mat(object *obj);

#endif