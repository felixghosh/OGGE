#ifndef GL_UTIL_H
#define GL_UTIL_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>

enum shadertype {VERTEX, FRAGMENT};


//OpenGL util functions
GLuint load_and_compile_shader(const char* filepath, enum shadertype type);

#endif