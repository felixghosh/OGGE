#include "gl_utils.h"
#include <stdio.h>
#include <stdlib.h>

GLuint VAOs[1];
GLuint Buffers[1];
const GLuint NumVertices = 6;



void init() {
    static const GLfloat vertices[6][3] = {
        {-0.90, -0.90, 0.0},
        {0.85, -0.90, 0.0},
        {-0.90, 0.85, 0.0},
        {0.90, -0.85, 0.0},
        {0.90, 0.90, 0.0},
        {-0.85, 0.90, 0.0}
    };

    glCreateBuffers(1, Buffers);
    glNamedBufferStorage(Buffers[0], sizeof(vertices), vertices, 0);

    unsigned int vertexShader = load_shader("shaders/def.vert", VERTEX);
    unsigned int fragmentShader = load_shader("shaders/def.frag", FRAGMENT);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Error! Program linking failed: %s\n", infoLog);
    }

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, VAOs);
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

}

void display() {
    static const float black[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glClearBufferfv(GL_COLOR, 0, black);

    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

int main(int argc, char *argv[]){
    
    if(!glfwInit()){
        printf("Could not initialize GLFW!\n");
        exit(1);
    } else {
        printf("GLFW initialized!\n");
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window) {
        printf("Could not create GLFW window!\n");
        exit(1);
    } else {
        printf("GLFW window created!\n");
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    } else {
        printf("GLEW initialized!\n");
    }
    printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    glfwSetKeyCallback(window, key_callback);
    

    init();
    
    while (!glfwWindowShouldClose(window)) {
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}