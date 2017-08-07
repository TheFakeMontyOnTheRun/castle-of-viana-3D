//
// Created by monty on 25/10/16.
//
#include <array>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
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
#include "CPlainFileLoader.h"

#include "CGame.h"
#include "commands/IGameCommand.h"

namespace Knights {

    IGameCommand::IGameCommand(std::shared_ptr<CGame> aGame) : mGame(aGame) {
    }

    std::ostream &operator<<(std::ostream &os, const IGameCommand &action) {

        os << to_string(action);
        return os;
    }

    std::shared_ptr<CGame> IGameCommand::getGame() {
        return mGame;
    }

    std::string to_string(const IGameCommand &action) {
        return action.to_string();
    }
}
