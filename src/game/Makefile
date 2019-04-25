CXXOPTFLAGS = -Wall -O2 -Wextra
CXXFLAGS = $(CXXOPTFLAGS) -I../../include `sdl-config --cflags` 
CLIENT_LIBS = `sdl-config --libs` -lSDL_image -lSDL_mixer -lz -lGL -lGLU -lX11 -lm -L../c-game -lcgame
LDLIBS =  -L../c-game -lcgame

CLIENT_OBJS = $(shell for i in *.cpp ; do echo $${i%.cpp}.o ; done )

all:

	$(MAKE) client

clean:

	-$(RM) $(SERVER_OBJS) $(CLIENT_OBJS) ../../bin/grafcube

client: $(CLIENT_OBJS)

	$(CXX) $(CXXFLAGS) -o ../../bin/grafcube $(CLIENT_OBJS) $(CLIENT_LIBS)

	
