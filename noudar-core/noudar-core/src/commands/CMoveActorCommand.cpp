//
// Created by monty on 25/10/16.
//
#include <array>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "Vec2i.h"
#include "IMapElement.h"
#include "CGameDelegate.h"
#include "CTeam.h"
#include "CItem.h"
#include "CActor.h"
#include "CMap.h"
#include "IRenderer.h"
#include "IFileLoaderDelegate.h"
#include "CPlainFileLoader.h"

#include "CGame.h"
#include "commands/IGameCommand.h"
#include "commands/CMoveActorCommand.h"

namespace Knights {

    CMoveActorCommand::CMoveActorCommand(std::shared_ptr<CGame> aGame, EDirection aDirection,
                                         std::shared_ptr<CActor> aActor) :
            IGameCommand(aGame),
            mDirection(aDirection),
            mActor(aActor) {
    }

    std::string CMoveActorCommand::to_string() const {

        std::stringstream ss;
        ss << "Move actor by ";
        ss << mDirection;
        return ss.str();
    }

    bool CMoveActorCommand::shouldEndTurn() {
        return true;
    }

    void CMoveActorCommand::execute() {
        std::shared_ptr<CMap> map = getGame()->getMap();

        map->move(mDirection, mActor);
    }
}
