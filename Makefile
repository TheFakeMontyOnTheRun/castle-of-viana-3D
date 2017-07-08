CXX = clang++

SDL_LIB = `sdl-config --libs` -lSDL_gfx -lSDL_image -lSDL_mixer
SDL_INCLUDE =  `sdl-config --cflags `

CXXFLAGS = -std=c++14 -O2 -g $(SDL_INCLUDE) -Iinclude -Istb
OBJS = src/CGame.o  src/CGameSnapshot.o SDLVersion/main.o SDLVersion/CSDLRenderer.o src/CRenderer.o src/Common.o src/NativeBitmap.o src/LoadPNG.o

LIBS = $(SDL_LIB)
TARGET = game

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:   $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
