#include "gl_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

GLuint load_and_compile_shader(const char* filepath, enum shadertype type) {
    FILE* fp = fopen(filepath, "r");
    if(fp == NULL){
        printf("Error! Shader source file can't be opened!\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END );
    int fileSize = ftell(fp);
    rewind(fp);

    char* shaderSource = (char*)malloc(sizeof(char)*fileSize+1);
    memset(shaderSource, 0, fileSize+1);
    int n = fread(shaderSource, sizeof(char), fileSize, fp);
    fclose(fp);

    unsigned int shader;
    if(type == VERTEX)
        shader = glCreateShader(GL_VERTEX_SHADER);
    else if(type == FRAGMENT)
        shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(shader, 1, (const char**)&shaderSource, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Error! Shader compilation failed: %s\n", infoLog);
    }
    free(shaderSource);

    return shader;
}

void print_vendor_info(int print_extensions) {
    const GLubyte *vendor, *renderer, *version, *extensions;
    vendor = glGetString(GL_VENDOR);
    renderer = glGetString(GL_RENDERER);
    version = glGetString(GL_VERSION);
    
    printf("Vendor: %s\nRenderer: %s\nVersion: %s\n", vendor, renderer, version);
    // Query for OpenGL Extensions
    if(print_extensions) {
        int i = 0;
        do {
            extensions = glGetStringi(GL_EXTENSIONS, i++);
            printf("Extension: %s\n", extensions);
        } while (extensions != NULL);
    }
}

void print_debug_messages() {
    int numMsgs = 100;
    GLint maxMsgLen = 0;
    glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);
    GLchar msgData[numMsgs * maxMsgLen];
    GLenum sources[numMsgs], types[numMsgs], severities[numMsgs];
    GLuint ids[numMsgs];
    GLsizei lengths[numMsgs];
    GLuint numFound = glGetDebugMessageLog(numMsgs, numMsgs * maxMsgLen, sources, types, ids, severities, lengths, msgData);

    printf("numFound: %u\n", numFound);
    int pos = 0;
    for(int i = 0; i < numFound; i++){
        for(int j = pos; j < pos + lengths[i]; j++){
            putchar(msgData[j]);
        }
        putchar('\n');
        pos += lengths[i];
    }
}