#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdio.h>

//Globals
int screenWidth = 680;
int screenHeight = 480;
SDL_Window *window = NULL;
SDL_GLContext opengl_context = NULL;
bool quit = false;

void getOpenGLVersionInfo(){
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "Could not initialize SDL2!\n");
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window =  SDL_CreateWindow("OpenGL Window", 0, 0, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
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

    printf("SDL and OpenGL initialized!\n");
    getOpenGLVersionInfo();
}

void handleInput() {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT)
            quit = true;
    }
}

void preDraw() {

}

void draw() {

}

void mainLoop() {
    while(!quit){
        handleInput();

        preDraw();

        draw();

        SDL_GL_SwapWindow(window);

    }
    printf("Program exit! Terminating application");
}

void terminate() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {

    init();

    mainLoop();

    terminate();

    return 0;
}