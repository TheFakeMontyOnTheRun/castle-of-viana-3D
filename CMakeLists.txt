cmake_minimum_required(VERSION 3.8)
project(castle_of_viana_3D)

find_package(SDL REQUIRED)

set(CMAKE_CXX_STANDARD 14)
include_directories(include)
include_directories(stb)
include_directories(fixed_point/include)
include_directories(core/noudar-core/include)
include_directories(core/EASTL/include )
include_directories(core/EASTL/test/packages/EABase/include/Common)

set(SOURCE_FILES
        SDLVersion/CSDLRenderer.cpp
        SDLVersion/main.cpp
        src/CRenderer.cpp
        src/RaycastCommon.cpp
        src/NativeBitmap.cpp
        src/LoadPNG.cpp
        include/CRenderer.h
        include/RaycastCommon.h
        core/noudar-core/include/commands/CMoveActorCommand.h
        core/noudar-core/include/commands/CEndTurnCommand.h
        core/noudar-core/include/commands/CLoadNewLevelCommand.h
        core/noudar-core/include/commands/CNullCommand.h
        core/noudar-core/include/commands/CUseCurrentItemCommand.h
        core/noudar-core/include/commands/CQuitCommand.h
        core/noudar-core/include/commands/CTurnActorCommand.h
        core/noudar-core/include/commands/CDropItemCommand.h
        core/noudar-core/include/commands/CPickItemCommand.h
        core/noudar-core/include/commands/IGameCommand.h
        core/noudar-core/include/CPlainFileLoader.h
        core/noudar-core/include/CActor.h
        core/noudar-core/include/CMonsterGenerator.h
        core/noudar-core/include/CMonster.h
        core/noudar-core/include/CDoorway.h
        core/noudar-core/include/CGame.h
        core/noudar-core/include/CMap.h
        core/noudar-core/include/IRenderer.h
        core/noudar-core/include/Vec2i.h
        core/noudar-core/include/CTeam.h
        core/noudar-core/include/CCharacterArchetype.h
        core/noudar-core/include/CCharacter.h
        core/noudar-core/src/commands/CMoveActorCommand.cpp
        core/noudar-core/src/commands/CEndTurnCommand.cpp
        core/noudar-core/src/commands/CLoadNewLevelCommand.cpp
        core/noudar-core/src/commands/CNullCommand.cpp
        core/noudar-core/src/commands/CQuitCommand.cpp
        core/noudar-core/src/commands/CTurnActorCommand.cpp
        core/noudar-core/src/commands/CCycleNextItemCommand.cpp
        core/noudar-core/src/commands/CPickItemCommand.cpp
        core/noudar-core/src/commands/CDropItemCommand.cpp
        core/noudar-core/src/commands/CCyclePreviousItemCommand.cpp
        core/noudar-core/src/commands/CUseCurrentItemCommand.cpp
        core/noudar-core/src/commands/IGameCommand.cpp
        core/noudar-core/src/CPlainFileLoader.cpp
        core/noudar-core/src/Vec2i.cpp
        core/noudar-core/src/Common.cpp
        core/noudar-core/src/CActor.cpp
        core/noudar-core/src/CItem.cpp
        core/noudar-core/include/CItem.h
        core/noudar-core/src/CCharacter.cpp
        core/noudar-core/src/CMonsterGenerator.cpp
        core/noudar-core/include/CMonsterGenerator.h
        core/noudar-core/src/CCharacterArchetype.cpp
        core/noudar-core/src/CGameDelegate.cpp
        core/noudar-core/src/CMonster.cpp
        core/noudar-core/src/IFileLoaderDelegate.cpp
        core/noudar-core/src/CTeam.cpp
        core/noudar-core/src/CDoorway.cpp
        core/noudar-core/src/CGame.cpp
        core/noudar-core/src/CMap.cpp
        core/noudar-core/src/IMapElement.cpp
        core/noudar-core/src/CStorageItem.cpp
        core/noudar-core/include/CStorageItem.h)

find_package(OpenGL REQUIRED)

include (FindPkgConfig)

add_executable(castle_of_viana_3D ${SOURCE_FILES})

target_link_libraries(castle_of_viana_3D ${OPENGL_LIBRARIES} ${SDL_LIBRARY}   )