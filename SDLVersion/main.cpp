#include <vector>
#include <functional>
#include <memory>
#include <iostream>
#include <array>
#include <utility>
#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <map>
#include "Vec2i.h"
#include "IMapElement.h"
#include "CTeam.h"
#include "CItem.h"
#include "CActor.h"
#include "CGameDelegate.h"
#include "CMap.h"
#include "IRenderer.h"
#include "IFileLoaderDelegate.h"
#include "CGame.h"
#include "CPlainFileLoader.h"
#include "Vec2i.h"
#include "IMapElement.h"
#include "CTeam.h"
#include "CItem.h"
#include "CActor.h"
#include "CGameDelegate.h"
#include "CMap.h"
#include "IRenderer.h"
#include "commands/IGameCommand.h"
#include "RaycastCommon.h"
#include "CRenderer.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

#endif

std::shared_ptr<odb::CRenderer> renderer;

void gameLoopTick() {
    renderer->render( 33 );
    renderer->handleSystemEvents();
}


int main() {

    const auto LEVEL_LIMIT = 2;
    renderer = std::make_shared<odb::CRenderer>();

    auto delegate = std::make_shared<Knights::CGameDelegate>();
    auto fileLoader = std::make_shared<Knights::CPlainFileLoader>();
    auto game = std::make_shared<Knights::CGame>( fileLoader, renderer, delegate );

    auto onLevelLoaded = [&]() {
        if ( game->getLevelNumber() >= LEVEL_LIMIT ) {
            game->setIsPlaying( false );
        }
    };

    delegate->setOnLevelLoadedCallback(onLevelLoaded );

#ifdef __EMSCRIPTEN__
    //  emscripten_request_fullscreen(0, 1);
  emscripten_set_main_loop( gameLoopTick, 30, 1 );
#else

    while ( game->isPlaying() ) {
        game->endOfTurn(game->getMap());
        game->tick();
        renderer->sleep( 33 );
        gameLoopTick();

    }
#endif
    return 0;
}
