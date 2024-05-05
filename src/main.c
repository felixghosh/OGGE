#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "gl_utils.h"
#include "object.h"
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "gcl_mem.h"

#include "vec.h"
#include "mat.h"
#include "transform.h"
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Globals
SDL_Window *window = NULL;
SDL_GLContext opengl_context = NULL;
bool quit = false;
GLuint fbo;
GLuint rbo;
GLuint rbod;
GLuint uniform_loc_time, uniform_loc_ctm;
object quad, cube;
double elapsed_time;
double game_time;
struct timespec t0, t1;
mat4 ctm;
float theta[3] = {0.0f, 0.0f, 0.0f};
enum {X, Y, Z} axis;

unsigned int renderWidth = 480;
unsigned int renderHeight = 270;
unsigned int windowWidth = 1600;
unsigned int windowHeight = 900;

void update_time()
{
  clock_gettime(CLOCK_REALTIME, &t1);
  elapsed_time = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1000000000.0;
  game_time += elapsed_time;
//   printf("fps: %5u\n", (int)(1 / elapsed_time));
  clock_gettime(CLOCK_REALTIME, &t0);
}

void getOpenGLVersionInfo(){
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void vertexSpecification() {
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
     1.0f,  1.0f, 0.0f,   // top left

    //tex-coords
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f
    };
    GLuint indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    quad.vertices = vertices;
    quad.indices = indices;
    quad.num_vertices = 6;

    //textures
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
    object_gen_textures(&quad, 1);
    object_bind_texture(&quad, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);


    object_gen_buffers(&quad);
    object_bind_buffers(&quad);

    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(24*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    uniform_loc_time = glGetUniformLocation(quad.shader_program, "time");


    //-------------------cube---------------------
    vec4 cube_vertices[16] = {
        {{-1.0, -1.0, -1.0, 1.0}}, {{ 1.0, -1.0, -1.0, 1.0}},
        {{ 1.0,  1.0, -1.0, 1.0}}, {{-1.0,  1.0, -1.0, 1.0}},
        {{-1.0, -1.0,  1.0, 1.0}}, {{1.0, -1.0,  1.0, 1.0}},
        {{ 1.0,  1.0,  1.0, 1.0}}, {{-1.0,  1.0,  1.0, 1.0}},

        {{0.0, 0.0, 0.0, 1.0}},{{1.0, 0.0, 0.0, 1.0}},
        {{1.0, 1.0, 0.0, 1.0}},{{0.0, 1.0, 0.0, 1.0}},
        {{0.0, 0.0, 1.0, 1.0}},{{1.0, 0.0, 1.0, 1.0}},
        {{0.0, 1.0, 1.0, 1.0}},{{1.0, 1.0, 1.0, 1.0}}
    };
    GLuint cube_indices[] = {
        0, 3, 2, 0, 2, 1,
        2, 3, 7, 2, 7, 6,
        3, 0, 4, 3, 4, 7,
        1, 2, 6, 1, 6, 5,
        4, 5, 6, 4, 6, 7,
        5, 4, 0, 5, 0, 1
    };

    cube.num_vertices = 36;
    object_gen_buffers(&cube);
    object_bind_buffers(&cube);

    glBufferData(GL_ARRAY_BUFFER, sizeof cube_vertices, cube_vertices, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof cube_indices, cube_indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(32*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    uniform_loc_ctm = glGetUniformLocation(cube.shader_program, "ctm");
    clock_gettime(CLOCK_REALTIME, &t0);
}

void createGraphicsPipeline(){
     //Set up FBO and RBO
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, renderWidth, renderHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);

    glGenRenderbuffers(1, &rbod);
    glBindRenderbuffer(GL_RENDERBUFFER, rbod);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, renderWidth, renderHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbod);
    //------------Load & compile shaders, attach and link to program---------
    object_attach_shaders(&quad, "shaders/quad.vert", "shaders/quad.frag");
    object_attach_shaders(&cube, "shaders/cube.vert", "shaders/cube.frag");
}

void init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "Could not initialize SDL2!\n");
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window =  SDL_CreateWindow("OpenGL Window", 0, 0, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
    if(!window){
        fprintf(stderr, "Could not create SDL window!\n");
        exit(1);
    }

    opengl_context = SDL_GL_CreateContext(window);
    if(!opengl_context){
        fprintf(stderr, "Could not create OpenGL context!\n");
        exit(1);
    }

    GLenum glew_init = glewInit();
    if (glew_init != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_init));
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    printf("SDL and OpenGL initialized!\n");
    getOpenGLVersionInfo();

    createGraphicsPipeline();
    vertexSpecification();
}

void handleInput() {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT)
            quit = true;
    }
}

void preDraw() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, renderWidth, renderHeight);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    theta[Z] += 51.0*elapsed_time;
    theta[Z] = theta[Z] > 360.0f ? theta[Z] - 360.0f : theta[Z];
    theta[Y] += 7.0*elapsed_time;
    theta[Y] = theta[Y] > 360.0f ? theta[Y] - 360.0f : theta[Y];
    theta[X] += 23.0*elapsed_time;
    theta[X] = theta[X] > 360.0f ? theta[X] - 360.0f : theta[X];
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void draw() {
    ctm = mat4_identity();
    mat4 rz = transform_rotate_z(theta[Z]);
    mat4 ry = transform_rotate_y(theta[Y]);
    mat4 rx = transform_rotate_x(theta[X]);
    mat4 tx = transform_translate(sin(game_time), 0.0f, 0.0f);
    mat4 ortho = camera_ortho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
    ctm = mat4_mul(ctm, rz);
    ctm = mat4_mul(ctm, ry);
    ctm = mat4_mul(ctm, rx);
    ctm = mat4_mul(ctm, ortho);

    float n = 0.1f;
    float f = 1000.0f;
    float t = tan(90.0)*n;
    float r = t*(16.0/9.0);
    mat4 p = {{
        {n/r, 0.0f, 0.0f, 0.0f},
        {0.0f, n/t, 0.0f, 0.0f},
        {0.0f, 0.0f, -((f+n)/(f-n)), ((-2*f*n)/(f-n))},
        {0.0f, 0.0f, -1.0f, 0.0f}
    }};
    // ctm = mat4_mul(ctm, p);
    // ctm = mat4_mul(ctm, tx);
    object_use(&cube);
    glUniformMatrix4fv(uniform_loc_ctm, 1, GL_TRUE, (const float *)ctm.m);
    object_render(&cube);
    // object_use(&quad);
    // glUniform1f(uniform_loc_time, game_time);
    // object_render(&quad);
}

void postDraw() {
    glViewport(0, 0, windowWidth, windowHeight);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, renderWidth, renderHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void mainLoop() {
    while(!quit){
        handleInput();

        update_time();

        preDraw();

        draw();

        postDraw();

        SDL_GL_SwapWindow(window);

    }
    printf("Program exit! Terminating application\n");
}

void terminate() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {

    init();

    mainLoop();

    terminate();

    check_allocations();

    return 0;
}