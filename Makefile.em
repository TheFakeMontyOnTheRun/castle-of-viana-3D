CXX = em++
CXXFLAGS = -O3 -g -DEA_PLATFORM_SAMSUNG_TV  --preload-file res --use-preload-plugins -Iinclude -Ifixed_point/include -std=c++14 -Istb -Icore/EASTL/include -Icore/EASTL/test/packages/EABase/include/Common -std=c++14 -Iinclude -Ifixed_point/include -Istb -Inoudar-core/noudar-core/include -ferror-limit=1  -s USE_ZLIB=1 -s USE_LIBPNG=1 -s ALLOW_MEMORY_GROWTH=1 -DUSE_ITEMS_INSTANTLY

LDFLAGS =  -O3 -s USE_ZLIB=1 -s USE_LIBPNG=1 -s --preload-file res --use-preload-plugins  -s ALLOW_MEMORY_GROWTH=1

CXX = em++
CC = emcc


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
	
TARGET = game.html

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(CXXFLAGS)

all:   $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	rm game.data
	rm game.js