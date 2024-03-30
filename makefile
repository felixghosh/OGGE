GLFW_FILES = glfw/src/main.c glfw/src/gl_utils.c
GLFW_LIBS = -lglfw -lGL -lGLEW

SDL_FILES = sdl/src/main.cpp sdl/src/gl_utils.c
SDL_LIBS = -lSDL2 -lGL -lGLEW -ldl 

BOGP_FILES = bogp/src/main.cpp
BOGP_LIBS = -lGL -lglfw -lGLEW

#Beginning OpenGL Game Programming
bogp: $(BOGP_FILES)
	g++ -g $(BOGP_FILES) $(BOGP_LIBS)
	./a.out

#SDL
sdl: $(SDL_FILES)
	g++ -g $(SDL_FILES) $(SDL_LIBS)
	./a.out


#GLFW
glfw: $(GLFW_FILES)
	gcc -g $(GLFW_FILES) $(GLFW_LIBS)
	./a.out