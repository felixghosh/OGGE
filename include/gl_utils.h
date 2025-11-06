#ifndef GL_UTIL_H
#define GL_UTIL_H

#include <GL/glew.h>

enum shadertype {VERTEX, FRAGMENT};

//Constants
#define SKYBOX_TEXTURE_UNIT  0
#define VERTEX_UBO_BINDING   0
#define FRAGMENT_UBO_BINDING 1


//OpenGL util functions
GLuint load_and_compile_shader(const char* filepath, enum shadertype type);

void print_vendor_info(int print_extensions);

void print_debug_messages();

#endif