OBJ = $(shell for i in *.c; do echo $${i%.c}.o ; done ) 

LDLIBS =  `sdl-config --libs` -lSDL_image -lSDL_mixer -lz -lGL -lGLU -lX11 -lm

CFLAGS = `sdl-config --cflags` -Wall -O3 -Wextra -I../../include -I../../include/c-game/ 

all: $(OBJ)

	$(AR) -cvq libcgame.a *.o 

clean:

	rm *.o libcgame.a 
	
