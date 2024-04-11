LIBS = -lSDL2 -lGL -lGLEW -ldl -lm -lgcl
FLAGS = -g -I include -L lib -Wall -Wextra -pedantic
OBJECTS = gl_utils object vec
CC = gcc

main: $(OBJECTS)
	$(CC) $(FLAGS) src/main.c obj/* $(LIBS)
	./a.out

gl_utils:
	$(CC) -c $(FLAGS) src/gl_utils.c -o obj/gl_utils.o $(LIBS)

object:
	$(CC) -c $(FLAGS) src/object.c -o obj/object.o $(LIBS)

vec:
	$(CC) -c $(FLAGS) src/vec.c -o obj/vec.o $(LIBS)