LIBS = -lSDL2 -lGL -lGLEW -lm -lgcl
FLAGS = -g -I include -L lib -Wall -Wextra -pedantic
OBJECTS = obj/gl_utils.o obj/object.o obj/vec.o obj/mat.o obj/transform.o obj/camera.o
CC = gcc

main: $(OBJECTS)
	$(CC) $(FLAGS) src/main.c $(OBJECTS) $(LIBS)
	./a.out

clear:
	rm a.out $(OBJECTS)

clean: clear main

obj/gl_utils.o: src/gl_utils.c
	$(CC) -c $(FLAGS) src/gl_utils.c -o obj/gl_utils.o $(LIBS)

obj/object.o: src/object.c
	$(CC) -c $(FLAGS) src/object.c -o obj/object.o $(LIBS)

obj/vec.o: src/vec.c
	$(CC) -c $(FLAGS) src/vec.c -o obj/vec.o $(LIBS)

obj/mat.o: src/mat.c
	$(CC) -c $(FLAGS) src/mat.c -o obj/mat.o $(LIBS)

obj/transform.o: src/transform.c
	$(CC) -c $(FLAGS) src/transform.c -o obj/transform.o $(LIBS)

obj/camera.o: src/camera.c
	$(CC) -c $(FLAGS) src/camera.c -o obj/camera.o $(LIBS)