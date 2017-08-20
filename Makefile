CXX = clang++

SDL_LIB = `sdl-config --libs` -lSDL_gfx -lSDL_image -lSDL_mixer
SDL_INCLUDE =  `sdl-config --cflags `

CXXFLAGS = -std=c++14 -O2 -g $(SDL_INCLUDE) -Iinclude -Ifixed_point/include -Istb -Inoudar-core/noudar-core/include

OBJS = SDLVersion/main.o \
        SDLVersion/CSDLRenderer.o \
        src/CRenderer.o \
        src/RaycastCommon.o \
        src/NativeBitmap.o \
        src/LoadPNG.o \
        noudar-core/noudar-core/src/Vec2i.o \
        noudar-core/noudar-core/src/CActor.o \
        noudar-core/noudar-core/src/CMonster.o \
        noudar-core/noudar-core/src/CMonsterGenerator.o \
        noudar-core/noudar-core/src/CTeam.o \
        noudar-core/noudar-core/src/CMap.o \
        noudar-core/noudar-core/src/Common.o \
        noudar-core/noudar-core/src/CHuntApproach.o \
        noudar-core/noudar-core/src/CCharacter.o \
        noudar-core/noudar-core/src/CCharacterArchetype.o \
        noudar-core/noudar-core/src/CDoorway.o \
        noudar-core/noudar-core/src/IFileLoaderDelegate.o \
        noudar-core/noudar-core/src/IMapElement.o \
        noudar-core/noudar-core/src/CGame.o \
        noudar-core/noudar-core/src/CItem.o \
        noudar-core/noudar-core/src/CStorageItem.o \
        noudar-core/noudar-core/src/CPlainFileLoader.o \
        noudar-core/noudar-core/src/CGameDelegate.o \
        noudar-core/noudar-core/src/commands/IGameCommand.o \
        noudar-core/noudar-core/src/commands/CMoveActorCommand.o \
        noudar-core/noudar-core/src/commands/CLoadNewLevelCommand.o \
        noudar-core/noudar-core/src/commands/CTurnActorCommand.o \
        noudar-core/noudar-core/src/commands/CQuitCommand.o \
        noudar-core/noudar-core/src/commands/CNullCommand.o \
        noudar-core/noudar-core/src/commands/CEndTurnCommand.o \
        noudar-core/noudar-core/src/commands/CPickItemCommand.o \
        noudar-core/noudar-core/src/commands/CDropItemCommand.o \
        noudar-core/noudar-core/src/commands/CUseCurrentItemCommand.o \
        noudar-core/noudar-core/src/commands/CCycleNextItemCommand.o \
        noudar-core/noudar-core/src/commands/CCyclePreviousItemCommand.o

LIBS = $(SDL_LIB)
TARGET = game

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:   $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
