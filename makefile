GLFW_FILES = glfw/src/main.c glfw/src/gl_utils.c glfw/src/object.c
GLFW_LIBS = -lglfw -lGL -lGLEW -lm

SDL_FILES = sdl/src/main.c sdl/src/gl_utils.c sdl/src/object.c sdl/src/vec.c
SDL_LIBS = -lSDL2 -lGL -lGLEW -ldl -lm
SDL_FLAGS = -g -I sdl/include

#SDL
default:
	gcc $(SDL_FLAGS) $(SDL_FILES) $(SDL_LIBS)

#GLFW
glfw: a.out
	gcc -g $(GLFW_FILES) $(GLFW_LIBS)
	./a.out