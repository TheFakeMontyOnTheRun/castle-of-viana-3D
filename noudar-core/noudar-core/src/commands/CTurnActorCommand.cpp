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
#include "commands/CTurnActorCommand.h"

namespace Knights {

    CTurnActorCommand::CTurnActorCommand(std::shared_ptr<CGame> aGame, EDirection aDirection,
                                         std::shared_ptr<CActor> aActor) :
            IGameCommand(aGame),
            mDirection(aDirection),
            mActor(aActor) {
    }

    std::string CTurnActorCommand::to_string() const {

        std::stringstream ss;
        ss << "Turn actor to ";
        ss << mDirection;
        return ss.str();
    }

    bool CTurnActorCommand::shouldEndTurn() {
        return false;
    }

    void CTurnActorCommand::execute() {

        mActor->setDirection(mDirection);
    }
}
