#include "gl_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "object.h"

struct object_t quad;
GLuint vboID;
GLuint vaoID;
GLuint eboID;
unsigned int shaderProgram;

unsigned int renderWidth = 320;
unsigned int renderHeight = 240;
unsigned int windowWidth = 1280;
unsigned int windowHeight = 720;

void init() {
    //--------Initial options and querying---------
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
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


    //------------Load & compile shaders---------
    object_attach_shaders(&quad,"bogp/shaders/def.vert", "bogp/shaders/def.frag");


    //------------Set up primitives-------------
    GLfloat vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f,   // top left
    
    //colors
     0.0f,  0.0f, 1.0f,  // top right
     0.0f,  1.0f, 0.0f,  // bottom right
     1.0f,  0.0f, 0.0f,  // bottom left
     1.0f,  1.0f, 0.0f   // top left  
    };
    GLuint indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    quad.vertices = vertices;
    quad.indices = indices;
    quad.num_vertices = 6; 
    
    object_generate_buffers(&quad);
    object_bind_buffers(&quad);

    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

void render() {
    object_render(&quad);
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
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OGGE:BOGP - v0.0.1", NULL, NULL);
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
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}