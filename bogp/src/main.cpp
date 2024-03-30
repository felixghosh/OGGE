#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

GLuint VAOs[1];
GLuint VBOs[1];
const GLuint NumVertices = 6;
GLuint fbo;
GLuint rbo;

unsigned int renderWidth = 320;
unsigned int renderHeight = 240;
unsigned int windowWidth = 1280;
unsigned int windowHeight = 720;



void init() {
    glEnable(GL_DEPTH_TEST);
    const GLubyte *vendor, *renderer, *version, *extensions;
    vendor = glGetString(GL_VENDOR);
    renderer = glGetString(GL_RENDERER);
    version = glGetString(GL_VERSION);
    
    printf("Vendor: %s\nRenderer: %s\nVersion: %s\n", vendor, renderer, version);
    // Query for OpenGL Extensions
    // int i = 0;
    // do {
    //     extensions = glGetStringi(GL_EXTENSIONS, i++);
    //     printf("Extension: %s\n", extensions);
    // } while (extensions != NULL);
}

void render() {
    
}

int main(int argc, char *argv[]){
    
    if(!glfwInit()){
        printf("Could not initialize GLFW!\n");
        exit(1);
    } else {
        printf("GLFW initialized!\n");
    }

    // glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "My Title", NULL, NULL);
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
    // glfwSetKeyCallback(window, key_callback);
    

    init();
    
    while (!glfwWindowShouldClose(window)) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}