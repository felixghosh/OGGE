#include "object.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "transform.h"
#include "gcl_mem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

object *object_create(){
    object *obj = checked_calloc(1, sizeof(object));
    return obj;
}

void object_load_obj(object *obj, const char *obj_filepath, const char *tex_filepath, vec4 color, vec3 position, float scale){
    //Load object with single texture
    if(tex_filepath != NULL) {
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(tex_filepath, &width, &height, &nrChannels, 0);
        object_gen_textures(obj, 1);
        object_bind_texture(obj, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else
        obj->textures = NULL;

    // obj->color = color;

    unsigned long int num_vertices = 0;
    unsigned long int num_normals = 0;
    unsigned long int num_tex_coords = 0;
    unsigned long int num_indices = 0;
    FILE *fp;
    fp = fopen(obj_filepath, "r");
    if(fp == NULL){
        fprintf(stderr, "ERROR: Unable to open file at %s", obj_filepath);
        exit(1);
    }

    size_t buf_size = BUFSIZ;
    char* buf = checked_malloc(buf_size*sizeof(char));
    char* endptr;

    do{
        getline(&buf, &buf_size, fp);
    } while(buf[0] != 'v');

    do{
        num_vertices++;
        getline(&buf, &buf_size, fp);
    } while(buf[0] == 'v' && buf[1] == ' ');

    do{
        num_normals++;
        getline(&buf, &buf_size, fp);
    } while(buf[0] == 'v' && buf[1] == 'n');

    do{
        num_tex_coords++;
        getline(&buf, &buf_size, fp);
    } while(buf[0] == 'v' && buf[1] == 't');
    while(getline(&buf, &buf_size, fp)>0)
        num_indices++;

    rewind(fp);

    obj->num_vertices = num_indices*3;
    obj->pos = position;
    obj->scale = scale;
    do{
        getline(&buf, &buf_size, fp);
    } while(buf[0] != 'v');

    vec4 *obj_vertices = checked_calloc(num_vertices, sizeof (vec4));
    vec4 *obj_colors = checked_calloc(num_vertices, sizeof (vec4));
    for(unsigned long i = 0; i < num_vertices; i++){
        endptr = buf;
        double values[3];
        for(int i = 0; i < 3; i++){
            values[i] = strtod(endptr+1, &endptr);
        }
        obj_vertices[i] = (vec4){{values[0], values[1], values[2], 1.0f}};
        obj_colors[i] = vec4_add(color, (vec4){{(i%3+1)/3, ((i+1)%3+1)/3, ((i+2)%3+1)/3, 0.0}});
        getline(&buf, &buf_size, fp);
    }

    vec3 *obj_normals = checked_calloc(num_normals, sizeof (vec3));
    for(unsigned long i = 0; i < num_normals; i++){
        endptr = buf;
        double values[3];
        values[0] = strtod(endptr+2, &endptr)*-1.0;
        values[1] = strtod(endptr+1, &endptr)*-1.0;
        values[2] = strtod(endptr+1, &endptr)*-1.0;
        obj_normals[i] = (vec3){{values[0], values[1], values[2]}};
        getline(&buf, &buf_size, fp);
    }

    vec2* obj_tex_coords = checked_calloc(num_tex_coords, sizeof (vec2));
    for(unsigned long i = 0; i < num_tex_coords; i++){

        endptr = buf;
        double values[2];
        values[0] = strtod(endptr+2, &endptr);
        values[1] = strtod(endptr+1, &endptr);
        obj_tex_coords[i] = (vec2){{values[0], values[1]}};
        getline(&buf, &buf_size, fp);
    }

    GLuint *obj_indices = checked_calloc(9*num_indices, sizeof (GLuint));
    for(unsigned long i = 0; i < num_indices; i++){
        getline(&buf, &buf_size, fp);
        endptr = buf;
        for(int j = 0; j < 3; j++){
            obj_indices[(i*3)+j] = strtol(endptr+1, &endptr, 10)-1;
            /*normal = */strtol(endptr+1, &endptr, 10)-1;
            /*tex_coord =*/strtol(endptr+1, &endptr, 10)-1;/**/
        }
    }

    object_gen_buffers(obj);
    object_bind_buffers(obj);

    GLsizeiptr vertices_size = num_vertices*4*sizeof(GLfloat);
    GLsizeiptr colors_size = num_vertices*4*sizeof(GLfloat);

    GLsizeiptr total_size = vertices_size + colors_size;

    glBufferData(GL_ARRAY_BUFFER, total_size, NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_size, obj_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, vertices_size, colors_size, obj_colors);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices*3*sizeof(GLuint), obj_indices, GL_DYNAMIC_DRAW);

    //Attribute 0 - vertex position
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    //Atribute 1 - vertex color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(num_vertices*4*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    checked_free(obj_vertices);
    checked_free(obj_colors);
    checked_free(obj_normals);
    checked_free(obj_tex_coords);
    checked_free(obj_indices);
    checked_free(buf);
}

void object_free(object *obj){
    if(obj->textures != NULL)
        checked_free(obj->textures);
    checked_free(obj);
}

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
    obj->textures = checked_malloc(num_textures * sizeof(GLuint));
    glGenTextures(num_textures, obj->textures);
}

void object_bind_texture(object *obj, unsigned int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, obj->textures[index]);
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