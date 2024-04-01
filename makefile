GLFW_FILES = glfw/src/main.c glfw/src/gl_utils.c glfw/src/object.c
GLFW_LIBS = -lglfw -lGL -lGLEW -lm

SDL_FILES = sdl/src/main.cpp sdl/src/gl_utils.c
SDL_LIBS = -lSDL2 -lGL -lGLEW -ldl 

#GLFW
glfw: $(GLFW_FILES)
	gcc -g $(GLFW_FILES) $(GLFW_LIBS)
	./a.out

#SDL
sdl: $(SDL_FILES)
	g++ -g $(SDL_FILES) $(SDL_LIBS)
	./a.out