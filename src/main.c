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

#include "stb_image.h"

// Globals
SDL_Window *window = NULL;
SDL_GLContext opengl_context = NULL;
bool quit = false;
GLuint fbo;
GLuint rbo;
GLuint rbod;
GLuint frag_ubo, vert_ubo;
typedef struct frag_ubo_data {
    vec3 light_pos;  float align_a;
    vec3 camera_pos; float align_b;
} frag_ubo_data;
typedef struct vert_ubo_data {
    mat4 model_mat;
    mat4 mvp;
    int is_textured;
} vert_ubo_data;

object *monkey, *cube, *room, *light;
vec3 light_pos = {{2.0, 5.0, -2.0}};
object *skybox;
GLuint skybox_tex;
double elapsed_time;
double game_time;
struct timespec t0, t1;
camera game_camera = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
bool wireframe = false;

SDL_GameController* controller_0;
SDL_Joystick* controller_0_joy;

unsigned int renderWidth = 480;
unsigned int renderHeight = 270;
unsigned int windowWidth = 1600;
unsigned int windowHeight = 900;


void update_time() {
    clock_gettime(CLOCK_REALTIME, &t1);
    elapsed_time = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1000000000.0;
    game_time += elapsed_time;
    printf("fps: %5u\n", (int)(1 / elapsed_time));
    printf("time_delta: %lfms\n", elapsed_time * 1000);
    clock_gettime(CLOCK_REALTIME, &t0);
}

void createGraphicsPipeline() {
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

    //Set up UBOs for vertex and fragment shaders
    glCreateBuffers(1, &frag_ubo);
    glNamedBufferStorage(frag_ubo, sizeof(frag_ubo_data), NULL, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, FRAGMENT_UBO_BINDING, frag_ubo);

    glCreateBuffers(1, &vert_ubo);
    glNamedBufferStorage(vert_ubo, sizeof(vert_ubo_data), NULL, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, VERTEX_UBO_BINDING, vert_ubo);

}

void objectSpecification() {
    //------------Set up primitives-------------

    // Create Objects. Load & compile shaders, attach and link to object program. Finally load OBJ-data from file and set up buffers and attributes
    cube = object_create();
    object_attach_shaders(cube, "shaders/default_vert.glsl", "shaders/default_frag.glsl");
    object_load_obj(cube, "models/cube.obj", NULL, (vec4){{1.0, 0.0, 0.0, 1.0}}, (vec3){{-1.0f, 1.0f, -2.0f}}, 1.0f);

    monkey = object_create();
    object_attach_shaders(monkey, "shaders/default_vert.glsl", "shaders/default_frag.glsl");
    object_load_obj(monkey, "models/monkey.obj", "textures/fur.jpg", (vec4){{0.0, 1.0, 0.0, 1.0}}, (vec3){{2.0f, 2.0f, -3.0f}}, 2.0f);

    room = object_create();
    object_attach_shaders(room, "shaders/default_vert.glsl", "shaders/default_frag.glsl");
    object_load_obj(room, "models/room.obj", "textures/container.jpg", (vec4){{0.0, 0.0, 1.0, 1.0}}, (vec3){{0.0f, -1.0f, 0.0f}}, 10.0f);

    //lights
    light = object_create();
    object_attach_shaders(light, "shaders/light_vert.glsl", "shaders/light_frag.glsl");
    object_load_obj(light, "models/sphere.obj", NULL, (vec4){{1.0, 0.8, 0.6, 1.0}}, (vec3){{2.0, 5.0, -2.0}}, 0.5);

    //Cube-mapped skybox
    skybox = object_create();
    object_attach_shaders(skybox, "shaders/skybox_vert.glsl", "shaders/skybox_frag.glsl");

    object_load_obj(skybox, "models/cube.obj", NULL, (vec4){{0.0, 0.0, 0.0, 0.0}}, (vec3){{0.0, 0.0, 0.0}}, 100);
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &skybox_tex);
    glTextureStorage2D(skybox_tex, 1, GL_RGBA8, 2048, 2048);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex);
    char *tex_paths[6] = {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg"
    };
    int width, height, nrChannels;
    unsigned char *data;
    stbi_set_flip_vertically_on_load(false);  
    for(unsigned int i = 0; i < 6; i++)
    {
        data = stbi_load(tex_paths[i], &width, &height, &nrChannels, 0);
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTextureSubImage3D(
            skybox_tex,
            0,                // mip level
            0, 0, i,          // x, y, z offset (z = face index)
            width, height, 1, // size (depth = 1)
            format,           // format of source data
            GL_UNSIGNED_BYTE, // type of source data
            data              // pointer to pixel data
        );
        stbi_image_free(data);
    }
    glTextureParameteri(skybox_tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(skybox_tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(skybox_tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(skybox_tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(skybox_tex, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    object_use(skybox);
    glUniform1i(glGetUniformLocation(skybox->shader_program, "skybox"), SKYBOX_TEXTURE_UNIT); 
}

void init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        fprintf(stderr, "Could not initialize SDL2!\n");
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GameControllerEventState(SDL_ENABLE);

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

    printf("\n\n\nSDL and OpenGL initialized!\n");
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    createGraphicsPipeline();
    objectSpecification();
    clock_gettime(CLOCK_REALTIME, &t0);

    int n = SDL_NumJoysticks();
    printf("Number of joysticks: %d\n", n);
    for (int i = 0; i < n; i++) {
    if (SDL_IsGameController(i)) {
        SDL_GameController *controller = SDL_GameControllerOpen(i);
        if (controller) {
            printf("Controller %d opened: %s\n", i, SDL_GameControllerName(controller));
            controller_0 = SDL_GameControllerOpen(0); // open controller 0
            controller_0_joy = SDL_JoystickOpen(0); // open the joysticks of controller 0 
        }
    }
}
}

void handleInput() {   
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    SDL_Event evt;
    while (SDL_PollEvent(&evt) != 0) {
        // Individual keypresses
        if (evt.type == SDL_KEYDOWN) {
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
        else if (evt.type == SDL_MOUSEMOTION) {
            Sint32 dx_mouse = evt.motion.xrel;
            Sint32 dy_mouse = evt.motion.yrel;
            //Mouse event polling seems to be independent of framerate. Avoid using elapsed time in calculation to get smooth camera movement
            camera_pitch(&game_camera, -dy_mouse * 0.1f);
            camera_yaw(&game_camera, -dx_mouse * 0.1f);
        }
        // Click
        else if (evt.type == SDL_MOUSEBUTTONDOWN) {
            // if (evt.button.button == SDL_BUTTON_LEFT)
            // {

            // }
        }
    }

    float player_speed = 10.0;
    // Multiple keypresses
    if (keystates[SDL_SCANCODE_W]) {
        game_camera.z    += cos(deg_to_rad(game_camera.theta_y)) * -1.0f * player_speed * elapsed_time;
        game_camera.x    += sin(deg_to_rad(game_camera.theta_y)) * -1.0f * player_speed * elapsed_time;
        skybox->pos.v[2] += cos(deg_to_rad(game_camera.theta_y)) * -1.0f * player_speed * elapsed_time;
        skybox->pos.v[0] += sin(deg_to_rad(game_camera.theta_y)) * -1.0f * player_speed * elapsed_time;
    }
    if (keystates[SDL_SCANCODE_S]) {
        game_camera.z    += cos(deg_to_rad(game_camera.theta_y)) * 1.0f * player_speed * elapsed_time;
        game_camera.x    += sin(deg_to_rad(game_camera.theta_y)) * 1.0f * player_speed * elapsed_time;
        skybox->pos.v[2] += cos(deg_to_rad(game_camera.theta_y)) * 1.0f * player_speed * elapsed_time;
        skybox->pos.v[0] += sin(deg_to_rad(game_camera.theta_y)) * 1.0f * player_speed * elapsed_time;
    }
    if (keystates[SDL_SCANCODE_A]) {
        game_camera.z    += cos(deg_to_rad(game_camera.theta_y) + M_PI / 2) * -1.0f * player_speed * elapsed_time;
        game_camera.x    += sin(deg_to_rad(game_camera.theta_y) + M_PI / 2) * -1.0f * player_speed * elapsed_time;
        skybox->pos.v[2] += cos(deg_to_rad(game_camera.theta_y) + M_PI / 2) * -1.0f * player_speed * elapsed_time;
        skybox->pos.v[0] += sin(deg_to_rad(game_camera.theta_y) + M_PI / 2) * -1.0f * player_speed * elapsed_time;
    }
    if (keystates[SDL_SCANCODE_D]) {
        game_camera.z    += cos(deg_to_rad(game_camera.theta_y) + M_PI / 2) * 1.0f * player_speed * elapsed_time;
        game_camera.x    += sin(deg_to_rad(game_camera.theta_y) + M_PI / 2) * 1.0f * player_speed * elapsed_time;
        skybox->pos.v[2] += cos(deg_to_rad(game_camera.theta_y) + M_PI / 2) * 1.0f * player_speed * elapsed_time;
        skybox->pos.v[0] += sin(deg_to_rad(game_camera.theta_y) + M_PI / 2) * 1.0f * player_speed * elapsed_time;
    }
    if (keystates[SDL_SCANCODE_R]) {
        game_camera.y    += player_speed * elapsed_time;
        skybox->pos.v[1] += player_speed * elapsed_time;
    }
    if (keystates[SDL_SCANCODE_F]) {
        game_camera.y    -= player_speed * elapsed_time;
        skybox->pos.v[1] -= player_speed * elapsed_time;
    }
    if (keystates[SDL_SCANCODE_Q]) {
        camera_yaw(&game_camera, 40.0f * elapsed_time);
    }
    if (keystates[SDL_SCANCODE_E]) {
        camera_yaw(&game_camera, -40.0f * elapsed_time);
    }
    if (keystates[SDL_SCANCODE_T]) {
        camera_pitch(&game_camera, 70.0f * elapsed_time);
    }
    if (keystates[SDL_SCANCODE_G]) {
        camera_pitch(&game_camera, -70.0f * elapsed_time);
    }

    //controller
    Sint16 left_joy_x = SDL_JoystickGetAxis(controller_0_joy, 0); // get axis 0 (left stick X)
    Sint16 left_joy_y = SDL_JoystickGetAxis(controller_0_joy, 1); // get axis 1 (left stick Y)
    Sint16 right_joy_x = SDL_JoystickGetAxis(controller_0_joy, 2); // get axis 2 (right stick X)
    Sint16 right_joy_y = SDL_JoystickGetAxis(controller_0_joy, 3); // get axis 3 (right stick Y)
    if(abs(left_joy_x) > 2000) {
        game_camera.z    += cos(deg_to_rad(game_camera.theta_y) + M_PI / 2) * 1.0f * player_speed * left_joy_x/20000.0f * elapsed_time;
        game_camera.x    += sin(deg_to_rad(game_camera.theta_y) + M_PI / 2) * 1.0f * player_speed * left_joy_x/20000.0f * elapsed_time;
        skybox->pos.v[2] += cos(deg_to_rad(game_camera.theta_y) + M_PI / 2) * 1.0f * player_speed * left_joy_x/20000.0f * elapsed_time;
        skybox->pos.v[0] += sin(deg_to_rad(game_camera.theta_y) + M_PI / 2) * 1.0f * player_speed * left_joy_x/20000.0f * elapsed_time;
    }
    if(abs(left_joy_y) > 2000) {
        game_camera.z    += cos(deg_to_rad(game_camera.theta_y)) * 1.0f * player_speed * left_joy_y/20000.0f * elapsed_time;
        game_camera.x    += sin(deg_to_rad(game_camera.theta_y)) * 1.0f * player_speed * left_joy_y/20000.0f * elapsed_time;
        skybox->pos.v[2] += cos(deg_to_rad(game_camera.theta_y)) * 1.0f * player_speed * left_joy_y/20000.0f * elapsed_time;
        skybox->pos.v[0] += sin(deg_to_rad(game_camera.theta_y)) * 1.0f * player_speed * left_joy_y/20000.0f * elapsed_time;
    }
    if(abs(right_joy_x) > 2000) {
        camera_yaw(&game_camera, -right_joy_x/200.0f * elapsed_time);
    }
    if(abs(right_joy_y) > 2000) {
        camera_pitch(&game_camera, -right_joy_y/200.0f * elapsed_time);
    }
    if (SDL_GameControllerGetButton(controller_0, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {
        game_camera.y    += player_speed * elapsed_time;
        skybox->pos.v[1] += player_speed * elapsed_time;
    }
    Sint16 r2 = SDL_GameControllerGetAxis(controller_0, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
    if(r2 > 2000) {
        game_camera.y    -= player_speed * elapsed_time;
        skybox->pos.v[1] -= player_speed * elapsed_time;
    }

    // printf("x:%2.2f y:%2.2f z:%2.2f yaw:%2.2f pitch:%2.2f\n",
    // game_camera.x, game_camera.y, game_camera.z, game_camera.theta_y ,game_camera.theta_x);
}

void preDraw() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, renderWidth, renderHeight);
    glClearColor(0.0/255.0, 0.0/255.0, 0.0/255.0, 1.0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void update_vertex_ubo_data(vert_ubo_data *v_data, object *obj, mat4 view, mat4 projection) {
    mat4 model, mvp;
    model = object_model_mat(obj);
    mvp = mat4_mul3(projection, view, model);

    v_data->model_mat = mat4_transpose(model);
    v_data->mvp = mat4_transpose(mvp);
    v_data->is_textured = (obj->textures != NULL);

    glNamedBufferSubData(vert_ubo, 0, sizeof(vert_ubo_data), v_data);
}

void draw() {   
    glFrontFace(GL_CCW);

    //UBO structs
    frag_ubo_data f_data;
    vert_ubo_data v_data;

    //----------Per frame fragment data----------
    light->pos.v[0] = sin(game_time) * 5.0;
    light->pos.v[1] = sin(game_time * 0.3) * 3.0;
    light->pos.v[2] = sin(game_time * 0.7) * 6.0;

    
    f_data.camera_pos = (vec3){{game_camera.x, game_camera.y, game_camera.z}};
    f_data.light_pos = light->pos;

    glNamedBufferSubData(frag_ubo, 0, sizeof(frag_ubo_data), &f_data);

    //----------Per frame vertex data----------
    mat4 view, projection;
    view = camera_view_mat(&game_camera, *cube);
    // projection = camera_ortho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
    // projection = camera_frustum(-1.0, 1.0, -1.0, 1.0, 0.5, 3.0);
    projection = camera_perspective(2.5f, 16.0f / 9.0f, 0.01, 1000.0);


    //----------Per model vertex data----------
    update_vertex_ubo_data(&v_data, cube, view, projection);
    object_render(cube);

    update_vertex_ubo_data(&v_data, monkey, view, projection);
    object_render(monkey);

    update_vertex_ubo_data(&v_data, room, view, projection);
    object_render(room);

    update_vertex_ubo_data(&v_data, light, view, projection);
    object_render(light);

    //Skybox manual handling. It is drawn from the inside, therefore the winding order of the triangles needs to be reversed
    glFrontFace(GL_CW);
    glBindTextureUnit(SKYBOX_TEXTURE_UNIT, skybox_tex);
    update_vertex_ubo_data(&v_data, skybox, view, projection);
    object_render(skybox);
}

void postDraw() {
    glViewport(0, 0, windowWidth, windowHeight);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, renderWidth, renderHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void mainLoop() {
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

void terminate() {
    object_free(cube);
    object_free(monkey);
    object_free(room);
    object_free(light);
    object_free(skybox);
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