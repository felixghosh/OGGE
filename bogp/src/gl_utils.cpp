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