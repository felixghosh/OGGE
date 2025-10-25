#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "gl_utils.h"
#include "object.h"
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "gcl_mem.h"
#include "gcl_error.h"

#include "vec.h"
#include "mat.h"
#include "transform.h"
#include "camera.h"

// Globals
SDL_Window *window = NULL;
SDL_GLContext opengl_context = NULL;
bool quit = false;
GLuint fbo;
GLuint rbo;
GLuint rbod;

object *monkey, *cube, *room, *light;
vec3 light_pos = {{2.0, 5.0, -2.0}};
double elapsed_time;
double game_time;
struct timespec t0, t1;
float theta[3] = {0.0f, 0.0f, 0.0f};
enum
{
    X,
    Y,
    Z
} axis;
camera game_camera = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
bool wireframe = false;

unsigned int renderWidth = 480;
unsigned int renderHeight = 270;
unsigned int windowWidth = 1600;
unsigned int windowHeight = 900;

void update_time()
{
    clock_gettime(CLOCK_REALTIME, &t1);
    elapsed_time = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1000000000.0;
    game_time += elapsed_time;
    // printf("fps: %5u\n", (int)(1 / elapsed_time));
    // printf("time_delta: %lfms\n", elapsed_time * 1000);
    clock_gettime(CLOCK_REALTIME, &t0);
}

void getOpenGLVersionInfo()
{
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void createGraphicsPipeline()
{
    // Set up FBO and RBO
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

    // Create Objects, Load & compile shaders, attach and link to object program
    cube = object_create();
    object_attach_shaders(cube, "shaders/default_vert.glsl", "shaders/default_frag.glsl");
    monkey = object_create();
    object_attach_shaders(monkey, "shaders/default_vert.glsl", "shaders/default_frag.glsl");
    room = object_create();
    object_attach_shaders(room, "shaders/default_vert.glsl", "shaders/default_frag.glsl");
    light = object_create();
    object_attach_shaders(light, "shaders/light_vert.glsl", "shaders/light_frag.glsl");
}

void vertexSpecification()
{
    //------------Set up primitives-------------

    // //------cube-------
    object_load_obj(cube, "models/cube.obj", NULL, (vec4){{1.0, 0.0, 0.0, 1.0}}, (vec3){{-1.0f, 1.0f, -2.0f}}, 1.0f);
    cube->uniform_loc_model = glGetUniformLocation(cube->shader_program, "model_mat");
    cube->uniform_loc_view = glGetUniformLocation(cube->shader_program, "view_mat");
    cube->uniform_loc_projection = glGetUniformLocation(cube->shader_program, "projection_mat");
    cube->uniform_loc_light_pos = glGetUniformLocation(cube->shader_program, "light_pos");
    cube->uniform_loc_camera_pos = glGetUniformLocation(cube->shader_program, "camera_pos");
    cube->uniform_loc_is_textured = glGetUniformLocation(cube->shader_program, "is_textured");

    object_load_obj(monkey, "models/monkey.obj", "textures/fur.jpg", (vec4){{0.0, 1.0, 0.0, 1.0}}, (vec3){{2.0f, 2.0f, -3.0f}}, 2.0f);
    monkey->uniform_loc_model = glGetUniformLocation(monkey->shader_program, "model_mat");
    monkey->uniform_loc_view = glGetUniformLocation(monkey->shader_program, "view_mat");
    monkey->uniform_loc_projection = glGetUniformLocation(monkey->shader_program, "projection_mat");
    monkey->uniform_loc_light_pos = glGetUniformLocation(monkey->shader_program, "light_pos");
    monkey->uniform_loc_camera_pos = glGetUniformLocation(monkey->shader_program, "camera_pos");
    monkey->uniform_loc_is_textured = glGetUniformLocation(monkey->shader_program, "is_textured");

    object_load_obj(room, "models/room.obj", "textures/container.jpg", (vec4){{0.0, 0.0, 1.0, 1.0}}, (vec3){{0.0f, -1.0f, 0.0f}}, 10.0f);
    room->uniform_loc_model = glGetUniformLocation(room->shader_program, "model_mat");
    room->uniform_loc_view = glGetUniformLocation(room->shader_program, "view_mat");
    room->uniform_loc_projection = glGetUniformLocation(room->shader_program, "projection_mat");
    room->uniform_loc_light_pos = glGetUniformLocation(room->shader_program, "light_pos");
    room->uniform_loc_camera_pos = glGetUniformLocation(room->shader_program, "camera_pos");
    room->uniform_loc_is_textured = glGetUniformLocation(room->shader_program, "is_textured");

    object_load_obj(light, "models/sphere.obj", NULL, (vec4){{1.0, 0.8, 0.6, 1.0}}, (vec3){{2.0, 5.0, -2.0}}, 0.5);
    light->uniform_loc_model = glGetUniformLocation(light->shader_program, "model_mat");
    light->uniform_loc_view = glGetUniformLocation(light->shader_program, "view_mat");
    light->uniform_loc_projection = glGetUniformLocation(light->shader_program, "projection_mat");
}

void init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Could not initialize SDL2!\n");
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("OpenGL Window", 0, 0, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
    if (!window)
    {
        fprintf(stderr, "Could not create SDL window!\n");
        exit(1);
    }

    opengl_context = SDL_GL_CreateContext(window);
    if (!opengl_context)
    {
        fprintf(stderr, "Could not create OpenGL context!\n");
        exit(1);
    }

    SDL_SetWindowMouseGrab(window, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    GLenum glew_init = glewInit();
    if (glew_init != GLEW_OK)
    {
        error_exit("Error: %s\n", glewGetErrorString(glew_init));
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
    clock_gettime(CLOCK_REALTIME, &t0);
}

void handleInput()
{
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    SDL_Event evt;
    while (SDL_PollEvent(&evt) != 0)
    {
        // Individual keypresses
        if (evt.type == SDL_KEYDOWN)
        {
            int keypressed = evt.key.keysym.sym;
            if (keypressed == SDLK_ESCAPE)
            {
                quit = true;
            }
            else if (keypressed == SDLK_l)
            { // l
                wireframe = !wireframe;
            }
            // else if (keypressed == SDLK_i)
            // { // i
            //     //Toggle debug state
            //     // debug = !debug;
            // }
        }
        // Mouse movement
        else if (evt.type == SDL_MOUSEMOTION)
        {
            int dx = evt.motion.xrel;
            int dy = evt.motion.yrel;
            // game_camera.theta_y += -dx * 3000 * elapsed_time;
            camera_pitch(&game_camera, -dy * 2000 * elapsed_time);
            camera_yaw(&game_camera, -dx * 2000 * elapsed_time);
        }
        // Click
        else if (evt.type == SDL_MOUSEBUTTONDOWN)
        {
            // int x = evt.motion.x;
            // int y = evt.motion.y;
            // if (evt.button.button == SDL_BUTTON_LEFT)
            // {

            // }
        }
    }

    // Multiple keypresses
    if (keystates[SDL_SCANCODE_W])
    {
        game_camera.z += cos(deg_to_rad(game_camera.theta_y)) * -1.0f * 2.0f * elapsed_time;
        game_camera.x += sin(deg_to_rad(game_camera.theta_y)) * -1.0f * 2.0f * elapsed_time;
    }
    if (keystates[SDL_SCANCODE_S])
    {
        game_camera.z += cos(deg_to_rad(game_camera.theta_y)) * 1.0f * 2.0f * elapsed_time;
        game_camera.x += sin(deg_to_rad(game_camera.theta_y)) * 1.0f * 2.0f * elapsed_time;
    }
    if (keystates[SDL_SCANCODE_A])
    {
        game_camera.z += cos(deg_to_rad(game_camera.theta_y) + M_PI / 2) * -1.0f * 2.0f * elapsed_time;
        game_camera.x += sin(deg_to_rad(game_camera.theta_y) + M_PI / 2) * -1.0f * 2.0f * elapsed_time;
    }
    if (keystates[SDL_SCANCODE_D])
    {
        game_camera.z += cos(deg_to_rad(game_camera.theta_y) + M_PI / 2) * 1.0f * 2.0f * elapsed_time;
        game_camera.x += sin(deg_to_rad(game_camera.theta_y) + M_PI / 2) * 1.0f * 2.0f * elapsed_time;
    }
    if (keystates[SDL_SCANCODE_R])
    {
        game_camera.y += 3.0f * elapsed_time;
    }
    if (keystates[SDL_SCANCODE_F])
    {
        game_camera.y -= 3.0f * elapsed_time;
    }
    if (keystates[SDL_SCANCODE_Q])
    {
        camera_yaw(&game_camera, 40.0f * elapsed_time);
    }
    if (keystates[SDL_SCANCODE_E])
    {
        camera_yaw(&game_camera, -40.0f * elapsed_time);
    }
    if (keystates[SDL_SCANCODE_T])
    {
        camera_pitch(&game_camera, 70.0f * elapsed_time);
    }
    if (keystates[SDL_SCANCODE_G])
    {
        camera_pitch(&game_camera, -70.0f * elapsed_time);
    }

    // printf("x:%2.2f y:%2.2f z:%2.2f yaw:%2.2f pitch:%2.2f\n",
    // game_camera.x, game_camera.y, game_camera.z, game_camera.theta_y ,game_camera.theta_x);
}

void preDraw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, renderWidth, renderHeight);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    theta[Z] += 51.0 * elapsed_time;
    theta[Z] = theta[Z] > 360.0f ? theta[Z] - 360.0f : theta[Z];
    theta[Y] += 7.0 * elapsed_time;
    theta[Y] = theta[Y] > 360.0f ? theta[Y] - 360.0f : theta[Y];
    theta[X] += 23.0 * elapsed_time;
    theta[X] = theta[X] > 360.0f ? theta[X] - 360.0f : theta[X];
    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void draw()
{
    mat4 model, view, projection;

    // mat4 rz = transform_rotate_z(theta[Z]);
    // mat4 ry = transform_rotate_y(theta[Y]);
    // mat4 rx = transform_rotate_x(theta[X]);

    model = object_model_mat(cube);
    // model = mat4_mul(model, rx);
    // model = mat4_mul(model, ry);
    // model = mat4_mul(model, rz);

    light->pos.v[0] = sin(game_time) * 5.0;
    light->pos.v[1] = sin(game_time * 0.3) * 3.0;
    light->pos.v[2] = sin(game_time * 0.7) * 6.0;

    view = camera_view_mat(&game_camera, *cube);

    // projection = camera_ortho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
    // projection = camera_frustum(-1.0, 1.0, -1.0, 1.0, 0.5, 3.0);
    projection = camera_perspective(2.2f, 16.0f / 9.0f, 0.01, 1000.0);

    object_use(cube);
    glUniformMatrix4fv(cube->uniform_loc_model, 1, GL_TRUE, (const float *)model.m);
    glUniformMatrix4fv(cube->uniform_loc_view, 1, GL_TRUE, (const float *)view.m);
    glUniformMatrix4fv(cube->uniform_loc_projection, 1, GL_TRUE, (const float *)projection.m);
    glUniform3fv(cube->uniform_loc_light_pos, 1, (const float *)light->pos.v);
    glUniform3f(cube->uniform_loc_camera_pos, game_camera.x, game_camera.y, game_camera.z);
    glUniform1i(cube->uniform_loc_is_textured, cube->textures != NULL);
    object_render(cube);

    model = object_model_mat(monkey);
    object_use(monkey);
    glUniformMatrix4fv(monkey->uniform_loc_model, 1, GL_TRUE, (const float *)model.m);
    glUniformMatrix4fv(monkey->uniform_loc_view, 1, GL_TRUE, (const float *)view.m);
    glUniformMatrix4fv(monkey->uniform_loc_projection, 1, GL_TRUE, (const float *)projection.m);
    glUniform3fv(monkey->uniform_loc_light_pos, 1, (const float *)light->pos.v);
    glUniform3f(monkey->uniform_loc_camera_pos, game_camera.x, game_camera.y, game_camera.z);
    glUniform1i(monkey->uniform_loc_is_textured, monkey->textures != NULL);
    object_render(monkey);

    model = object_model_mat(room);
    object_use(room);
    glUniformMatrix4fv(room->uniform_loc_model, 1, GL_TRUE, (const float *)model.m);
    glUniformMatrix4fv(room->uniform_loc_view, 1, GL_TRUE, (const float *)view.m);
    glUniformMatrix4fv(room->uniform_loc_projection, 1, GL_TRUE, (const float *)projection.m);
    glUniform3fv(room->uniform_loc_light_pos, 1, (const float *)light->pos.v);
    glUniform3f(room->uniform_loc_camera_pos, game_camera.x, game_camera.y, game_camera.z);
    glUniform1i(room->uniform_loc_is_textured, room->textures != NULL);
    object_render(room);

    model = object_model_mat(light);
    object_use(light);
    glUniformMatrix4fv(light->uniform_loc_model, 1, GL_TRUE, (const float *)model.m);
    glUniformMatrix4fv(light->uniform_loc_view, 1, GL_TRUE, (const float *)view.m);
    glUniformMatrix4fv(light->uniform_loc_projection, 1, GL_TRUE, (const float *)projection.m);
    glUniform3fv(light->uniform_loc_light_pos, 1, (const float *)light->pos.v);
    glUniform3f(light->uniform_loc_camera_pos, game_camera.x, game_camera.y, game_camera.z);
    object_render(light);
    // vec3_print(light->pos);
}

void postDraw()
{
    glViewport(0, 0, windowWidth, windowHeight);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, renderWidth, renderHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void mainLoop()
{
    while (!quit)
    {
        handleInput();

        update_time();

        preDraw();

        draw();

        postDraw();

        SDL_GL_SwapWindow(window);
    }
    printf("Program exit! Terminating application\n");
}

void terminate()
{
    object_free(cube);
    object_free(monkey);
    object_free(room);
    object_free(light);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{

    init();

    mainLoop();

    terminate();

    check_allocations();

    return 0;
}