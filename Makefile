CXX = clang++

SDL_LIB = `sdl-config --libs` -lSDL_gfx -lSDL_image -lSDL_mixer
SDL_INCLUDE =  `sdl-config --cflags `

CXXFLAGS = -std=c++14 -O2 -g -Wall -fno-rtti -Bstatic -Weffc++ -fmessage-length=0 $(SDL_INCLUDE) -Iinclude
OBJS = src/CGame.o  src/CGameSnapshot.o SDLVersion/main.o SDLVersion/CSDLRenderer.o src/CRenderer.o
LIBS = $(SDL_LIB)
TARGET = game

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:   $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
