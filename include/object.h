#ifndef OBJECT_H
#define OBJECT_H

#include "gl_utils.h"
#include "vec.h"
#include "mat.h"

typedef struct object {
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    GLuint shader_program;
    unsigned long int num_vertices;
    GLuint *textures;
    unsigned int color;
    vec3 pos;
    float scale;
    vec3 orientation;
    GLuint uniform_loc_time;
    GLuint uniform_loc_model;
    GLuint uniform_loc_view;
    GLuint uniform_loc_projection;
    GLuint uniform_loc_light_pos;
    GLuint uniform_loc_camera_pos;
    GLuint uniform_loc_is_textured;
} object;

void    object_load_obj(object *obj, const char *obj_filepath, const char *tex_filepath, vec4 color, vec3 position, float scale);

object *object_create();

void    object_free(object *obj);

void    object_attach_shaders(object *obj, const char *vertex_shader_path, const char *fragment_shader_path);

void    object_render(object *obj);

void    object_use(object *obj);

void    object_gen_buffers(object *obj);

void    object_gen_textures(object *obj, unsigned int num_textures);

void    object_bind_texture(object *obj, unsigned int index);

mat4    object_model_mat(object *obj);

#endif