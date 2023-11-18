GLFW_FILES = glfw/src/main.c glfw/src/gl_utils.c
GLFW_LIBS = -lglfw -lGL -lGLEW

SDL_FILES = sdl/src/main.cpp
SDL_LIBS = -lSDL2 -lGL -lGLEW -ldl 

#GLFW
default: $(GLFW_FILES)
	gcc -g $(GLFW_FILES) $(GLFW_LIBS)
	./a.out

#SDL
sdl: $(SDL_FILES)
	g++ -g $(SDL_FILES) $(SDL_LIBS)
	./a.out