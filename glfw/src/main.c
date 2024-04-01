#include "gl_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "object.h"


GLuint fbo;
GLuint rbo;
GLuint uniform_loc;
struct timespec t0, t1;
object_t quad;

unsigned int renderWidth = 320;
unsigned int renderHeight = 240;
unsigned int windowWidth = 1280;
unsigned int windowHeight = 720;



void init() {
    //Set up FBO and RBO
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, renderWidth, renderHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);


    //--------Initial options and querying---------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
    print_vendor_info(0);


    //------------Load & compile shaders---------
    object_attach_shaders(&quad,"glfw/shaders/def.vert", "glfw/shaders/def.frag");


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
    
    glGenVertexArrays(1, &(quad.vao));
    glGenBuffers(1, &(quad.vbo));
    glGenBuffers(1, &(quad.ebo));

    glBindVertexArray(quad.vao);
    glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    uniform_loc = glGetUniformLocation(quad.shader_program, "b_comp");
    clock_gettime(CLOCK_REALTIME, &t0);
}

void render() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, renderWidth, renderHeight);

    //update time;
    static double time_sum = 0.0;
    clock_gettime(CLOCK_REALTIME, &t1);
    double elapsed_time = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1000000000.0;
    time_sum += elapsed_time;
    clock_gettime(CLOCK_REALTIME, &t0);
    float uf = sin(glfwGetTime()) / 2.0f + 0.5f;


    glUseProgram(quad.shader_program);

    // static const float black[] = {0.0f, 0.0f, 0.0f, 0.0f};
    // glClearBufferfv(GL_COLOR, 0, black);
    

    glBindVertexArray(quad.vao);
    glUniform1f(uniform_loc, uf);

    // glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glDrawElements(GL_TRIANGLES, quad.num_vertices, GL_UNSIGNED_INT, 0);

    glViewport(0, 0, windowWidth, windowHeight);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, renderWidth, renderHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

int main(int argc, char *argv[]){
    
    if(!glfwInit()){
        printf("Could not initialize GLFW!\n");
        exit(1);
    }
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OGGE:BOGP - v0.0.1", NULL, NULL);
    if (!window) {
        printf("Could not create GLFW window!\n");
        exit(1);
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    glfwSetKeyCallback(window, key_callback);
    

    init();
    
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    print_debug_messages();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}