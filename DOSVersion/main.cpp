#include <functional>
#include <memory>
#include <vector>
#include <iostream>
#include "Common.h"
#include "CGameSnapshot.h"
#include "CGame.h"
#include "CRenderer.h"

std::shared_ptr<odb::CRenderer> renderer;
odb::CGame game;

void gameLoopTick() {
    game.tick( 33 );
    renderer->setSnapshot( game.getGameSnapshot() );
    renderer->render( 33 );
    renderer->handleSystemEvents();
}


int main() {
    renderer = std::make_shared<odb::CRenderer>( game.getKeyPressedCallback(), game.getKeyReleasedCallback() );


    while ( true ) {
        gameLoopTick();
        renderer->sleep( 33 );
    }

    return 0;
}
