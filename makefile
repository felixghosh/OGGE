FILES = src/main.c src/gl_utils.c
LIBS = -lGL -lglfw -lGLEW

default: $(FILES)
	gcc -g $(FILES) $(LIBS)
	./a.out