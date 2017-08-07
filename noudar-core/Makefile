CXXFLAGS = -Inoudar-core/include -Incurses-version -O2 -g -c -std=c++0x  -ferror-limit=1 -Werror -Wall --pedantic \
		-I$(GTEST_DIR)/include \
		-I$(GTEST_DIR) \
		-I$(GTEST_DIR)/include/gtest \
		-I$(GTEST_DIR)/include/gtest/internal \
		-I$(GMOCK_DIR)/include \
		-I$(GMOCK_DIR) \
		-I$(GMOCK_DIR)/include/gtest \
		-I$(GMOCK_DIR)/include/gtest/internal

CXX = clang++


GTEST_DIR = lib/googletest/googletest
GMOCK_DIR = lib/googletest/googlemock



TESTOBJS = Tests/TestCCharacter.o \
		Tests/TestCGame.o \
		Tests/TestVec2i.o \
		$(GTEST_DIR)/src/gtest-all.o \
		$(GMOCK_DIR)/src/gmock-all.o \
		$(GMOCK_DIR)/src/gmock_main.o



OBJS = ncurses-version/CConsoleRenderer.o \
    noudar-core/src/Vec2i.o \
    noudar-core/src/CActor.o \
    noudar-core/src/CMonster.o \
    noudar-core/src/CTeam.o \
    noudar-core/src/CMap.o \
    noudar-core/src/CHuntApproach.o \
    noudar-core/src/CCharacter.o \
    noudar-core/src/CCharacterArchetype.o \
    noudar-core/src/CDoorway.o \
    noudar-core/src/Common.o \
    noudar-core/src/IFileLoaderDelegate.o \
    noudar-core/src/IMapElement.o \
    noudar-core/src/CGame.o \
    noudar-core/src/CMonsterGenerator.o \
    noudar-core/src/CRandomWorldGenerator.o \
    noudar-core/src/CItem.o \
    noudar-core/src/CStorageItem.o \
    noudar-core/src/CPlainFileLoader.o \
    noudar-core/src/CGameDelegate.o \
    noudar-core/src/commands/CCycleNextItemCommand.o \
    noudar-core/src/commands/CCyclePreviousItemCommand.o \
    noudar-core/src/commands/IGameCommand.o \
    noudar-core/src/commands/CMoveActorCommand.o \
    noudar-core/src/commands/CLoadNewLevelCommand.o \
    noudar-core/src/commands/CTurnActorCommand.o \
    noudar-core/src/commands/CQuitCommand.o \
    noudar-core/src/commands/CDropItemCommand.o \
    noudar-core/src/commands/CPickItemCommand.o \
    noudar-core/src/commands/CNullCommand.o \
    noudar-core/src/commands/CUseCurrentItemCommand.o \
    noudar-core/src/commands/CEndTurnCommand.o

MAIN_GAME_OBJ = ncurses-version/main.o

LDFLAGS = -lncurses
TESTLDFLAGS = -lpthread
TARGET = noudar
TESTTARGET = unittests


$(TARGET):	$(OBJS) $(MAIN_GAME_OBJ)
	$(CXX) -o $(TARGET) $(OBJS) $(MAIN_GAME_OBJ) $(LDFLAGS)

$(TESTTARGET): $(OBJS) $(TESTOBJS)
	$(CXX) -o $(TESTTARGET) $(OBJS) $(TESTOBJS) $(TESTLDFLAGS) $(LDFLAGS)

all:	$(TARGET)


clean:
	rm -f $(OBJS) $(TESTTARGET) $(MAIN_GAME_OBJ) $(TESTOBJS) $(TARGET)
	find . | grep ~ | xargs rm
	find . | grep gcno | xargs rm
	find . | grep gcda | xargs rm
