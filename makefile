LIBS = -lSDL2 -lGL -lGLEW -ldl -lm -lgcl
FLAGS = -g -I include -L lib -Wall -Wextra -pedantic
OBJECTS = obj/gl_utils.o obj/object.o obj/vec.o
CC = gcc

main: $(OBJECTS)
	$(CC) $(FLAGS) src/main.c $(OBJECTS) $(LIBS)
	./a.out

clear:
	rm a.out $(OBJECTS)

clean: clear main

obj/gl_utils.o:
	$(CC) -c $(FLAGS) src/gl_utils.c -o obj/gl_utils.o $(LIBS)

obj/object.o:
	$(CC) -c $(FLAGS) src/object.c -o obj/object.o $(LIBS)

obj/vec.o:
	$(CC) -c $(FLAGS) src/vec.c -o obj/vec.o $(LIBS)