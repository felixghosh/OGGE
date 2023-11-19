#include "gl_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    GLuint shader;
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