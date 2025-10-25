#include "object.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "transform.h"
#include "gcl_mem.h"
#include "gcl_error.h"

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
        error_exit("ERROR: Unable to open file at %s\n", obj_filepath);
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

    // printf("num_vertices: %lu\nnum_normals: %lu\nnum_txcrds: %lu\nnum_indices: %lu\n\n", num_vertices, num_normals, num_tex_coords, num_indices);

    obj->num_vertices = num_indices*3;
    obj->pos = position;
    obj->scale = scale;
    do{
        getline(&buf, &buf_size, fp);
    } while(buf[0] != 'v');

    vec4 *vertices = checked_calloc(num_vertices, sizeof (vec4));
    for(unsigned long i = 0; i < num_vertices; i++){
        endptr = buf;
        double values[3];
        for(int i = 0; i < 3; i++){
            values[i] = strtod(endptr+1, &endptr);
        }
        vertices[i] = (vec4){{values[0], values[1], values[2], 1.0f}};
        getline(&buf, &buf_size, fp);
    }

    vec3 *normals = checked_calloc(num_normals, sizeof (vec3));
    for(unsigned long i = 0; i < num_normals; i++){
        endptr = buf;
        float values[3];
        values[0] = strtof(endptr+2, &endptr);
        values[1] = strtof(endptr+1, &endptr);
        values[2] = strtof(endptr+1, &endptr);
        normals[i] = (vec3){{values[0], values[1], values[2]}};
        getline(&buf, &buf_size, fp);
    }

    vec2* tex_coords = checked_calloc(num_tex_coords, sizeof (vec2));
    for(unsigned long i = 0; i < num_tex_coords; i++){

        endptr = buf;
        double values[2];
        values[0] = strtod(endptr+2, &endptr);
        values[1] = strtod(endptr+1, &endptr);
        tex_coords[i] = (vec2){{values[0], values[1]}};
        getline(&buf, &buf_size, fp);
    }

    GLuint *obj_indices = checked_calloc(3*num_indices, sizeof (GLuint));
    vec4 *obj_vertices = checked_calloc(3*num_indices, sizeof (vec4));
    vec4 *obj_colors = checked_calloc(3*num_indices, sizeof (vec4));
    vec3 *obj_normals = checked_calloc(3*num_indices, sizeof (vec3));
    vec2 *obj_tex_coords = checked_calloc(3*num_indices, sizeof (vec2));
    for(unsigned long i = 0; i < num_indices; i++){
        getline(&buf, &buf_size, fp);
        endptr = buf;
        for(int j = 0; j < 3; j++){
            unsigned long long vertice_index = strtoll(endptr+1, &endptr, 10)-1;
            obj_vertices[(i*3)+j] = vertices[vertice_index];
            obj_colors[(i*3)+j] = color;
            obj_indices[(i*3)+j] = vertice_index;
            unsigned long long tex_coord_index = strtoll(endptr+1, &endptr, 10)-1;
            obj_tex_coords[(i*3)+j] = tex_coords[tex_coord_index];
            unsigned long long normal_index = strtoll(endptr+1, &endptr, 10)-1;
            obj_normals[(i*3)+j] = normals[normal_index];
        }
    }
    // for(unsigned long i = 0; i < num_indices*3; i++){
    //     printf("%lu: ", i);
    //     vec4_print(obj_vertices[i]);
    // }
    // printf("\n");
    // for(unsigned long i = 0; i < num_indices*3; i++){
    //     printf("%lu: ", i);
    //     vec4_print(obj_colors[i]);
    // }
    // printf("\n");
    // for(unsigned long i = 0; i < num_indices*3; i++){
    //     printf("%lu: ", i);
    //     vec3_print(obj_normals[i]);
    // }
    // printf("\n");
    // for(unsigned long i = 0; i < num_indices*3; i++){
    //     printf("%lu: ", i);
    //     vec2_print(obj_tex_coords[i]);
    // }

    //TODO: You might consider rewriting the buffer layout above to let all vertex attribute reside in one contiguous region instead of one region per attribute
    //Using this kind of interleaved layout would probably lead to fewer memory accesses when the GPU is fethcing vertex attributes :)
    object_gen_buffers(obj);

    GLsizeiptr vertices_size = 3*num_indices*4*sizeof(GLfloat);
    GLsizeiptr colors_size = 3*num_indices*4*sizeof(GLfloat);
    GLsizeiptr normals_size = 3*num_indices*3*sizeof(GLfloat);
    GLsizeiptr tex_coords_size = 3*num_indices*2*sizeof(GLfloat);
    GLsizeiptr total_size = vertices_size + colors_size + normals_size + tex_coords_size;

    glNamedBufferStorage(obj->vbo, total_size, NULL, GL_DYNAMIC_STORAGE_BIT); //allocate the buffer object's immutable data store. The flag makes it so we can update the data with glBufferSubData
    glNamedBufferSubData(obj->vbo, 0, vertices_size, obj_vertices);
    glNamedBufferSubData(obj->vbo, vertices_size, colors_size, obj_colors);
    glNamedBufferSubData(obj->vbo, vertices_size+colors_size, normals_size, obj_normals);
    glNamedBufferSubData(obj->vbo, vertices_size+colors_size+normals_size, tex_coords_size, obj_tex_coords);
    // glNamedBufferStorage(obj->ebo num_indices*3*sizeof(GLuint), obj_indices, 0/*GL_DYNAMIC_STORAGE_BIT*/);  //allocate and initialize element buffer object

    //Attribute 0 - vertex position
    GLint vertex_pos_attrib_index = glGetAttribLocation(obj->shader_program, "vPos");
    GLuint vertex_pos_binding_index = vertex_pos_attrib_index;
    GLint nbr_vertex_positions = 4; //xyzw
    glVertexArrayVertexBuffer(obj->vao, vertex_pos_binding_index, obj->vbo, 0, nbr_vertex_positions * sizeof(GL_FLOAT));
    glVertexArrayAttribFormat(obj->vao, vertex_pos_attrib_index, nbr_vertex_positions, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(obj->vao, vertex_pos_attrib_index);
    glVertexArrayAttribBinding(obj->vao, vertex_pos_attrib_index, vertex_pos_binding_index);

    //Atribute 1 - vertex color
    GLint vertex_color_attrib_index = glGetAttribLocation(obj->shader_program, "vColor");
    GLuint vertex_color_binding_index = vertex_color_attrib_index;
    GLint nbr_vertex_colors = 4; //rgba
    glVertexArrayVertexBuffer(obj->vao, vertex_color_binding_index, obj->vbo, vertices_size, nbr_vertex_colors * sizeof(GL_FLOAT));
    glVertexArrayAttribFormat(obj->vao, vertex_color_attrib_index, nbr_vertex_colors, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(obj->vao, vertex_color_attrib_index);
    glVertexArrayAttribBinding(obj->vao, vertex_color_attrib_index, vertex_color_binding_index);

    //Atribute 2 - vertex normal
    GLint vertex_normal_attrib_index = glGetAttribLocation(obj->shader_program, "vNormal");
    GLuint vertex_normal_binding_index = vertex_normal_attrib_index;
    GLint nbr_vertex_normals = 3; //xyz
    glVertexArrayVertexBuffer(obj->vao, vertex_normal_binding_index, obj->vbo, (vertices_size+colors_size), nbr_vertex_normals * sizeof(GL_FLOAT));
    glVertexArrayAttribFormat(obj->vao, vertex_normal_attrib_index, nbr_vertex_normals, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(obj->vao, vertex_normal_attrib_index);
    glVertexArrayAttribBinding(obj->vao, vertex_normal_attrib_index, vertex_normal_binding_index);

    //Atribute 3 - texture coordinates
    GLint vertex_texcoord_attrib_index = glGetAttribLocation(obj->shader_program, "vTexCoord");
    GLuint vertex_texcoord_binding_index = vertex_texcoord_attrib_index;
    GLint nbr_tex_coords = 2; //uv
    glVertexArrayVertexBuffer(obj->vao, vertex_texcoord_binding_index, obj->vbo, (vertices_size+colors_size+normals_size), nbr_tex_coords * sizeof(GL_FLOAT));
    glVertexArrayAttribFormat(obj->vao, vertex_texcoord_attrib_index, nbr_tex_coords, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(obj->vao, vertex_texcoord_attrib_index);
    glVertexArrayAttribBinding(obj->vao, vertex_texcoord_attrib_index, vertex_texcoord_binding_index);

    checked_free(vertices);
    checked_free(obj_vertices);
    checked_free(obj_colors);
    checked_free(normals);
    checked_free(obj_normals);
    checked_free(tex_coords);
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
        warning("Warning! Program linking failed: %s\n", infoLog);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void object_render(object *obj) {
    glBindVertexArray(obj->vao);
    glDrawArrays(GL_TRIANGLES, 0, obj->num_vertices);
}

void object_use(object *obj) {
    glUseProgram(obj->shader_program);
    glBindVertexArray(obj->vao);
    if(obj->textures != NULL)
        object_bind_texture(obj, 0);
}

void object_gen_buffers(object *obj) {
    glCreateVertexArrays(1, &(obj->vao));
    glCreateBuffers(1, &(obj->vbo));
    // glCreateBuffers(1, &(obj->ebo)); //TODO - fix EBO implementation. Rewrite object_load_obj function to not manually copy each vertex for each triangle.
    //Instead just store each vertex in the buffer once, store the number of indices in the object, and replace glDrawArrays with glDrawElements
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