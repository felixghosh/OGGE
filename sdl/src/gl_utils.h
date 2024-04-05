#ifndef GL_UTIL_H
#define GL_UTIL_H

#include <GL/glew.h>

enum shadertype {VERTEX, FRAGMENT};


//OpenGL util functions
GLuint load_and_compile_shader(const char* filepath, enum shadertype type);

void print_vendor_info(int print_extensions);

void print_debug_messages();

#endif