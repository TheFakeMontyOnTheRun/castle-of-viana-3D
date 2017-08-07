/*
  main.c
  The Knights from Porto

  Created by Daniel Monteiro on 11/26/14.
  Copyright (c) 2014 Daniel Monteiro. All rights reserved.
*/

#include <utility>
#include <functional>
#include <string>
#include <iostream>
#include <memory>
#include <array>
#include <fstream>
#include <vector>
#include <map>

#include "IFileLoaderDelegate.h"
#include "CPlainFileLoader.h"

#include "Vec2i.h"
#include "IMapElement.h"
#include "CTeam.h"
#include "CItem.h"
#include "CActor.h"
#include "CGameDelegate.h"
#include "CMap.h"
#include "IRenderer.h"
#include "CConsoleRenderer.h"
#include "CGame.h"
#include "commands/IGameCommand.h"


int main ( int argc, char **argv ) {
    const auto LEVEL_LIMIT = 8;

    auto delegate = std::make_shared<Knights::CGameDelegate>();
    auto fileLoader = std::make_shared<Knights::CPlainFileLoader>();
    auto game = std::make_shared<Knights::CGame>( fileLoader, std::make_shared<Knights::CConsoleRenderer>(), delegate );

    auto onLevelLoaded = [&]() {
        if ( game->getLevelNumber() >= LEVEL_LIMIT ) {
            game->setIsPlaying( false );
        }
    };

    delegate->setOnLevelLoadedCallback(onLevelLoaded );

    while ( game->isPlaying() ) {
        game->tick();
    }

  return 0;
}
