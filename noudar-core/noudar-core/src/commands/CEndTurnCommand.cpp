//
// Created by monty on 25/10/16.
//
#include <array>
#include <string>
#include <vector>
#include <memory>
#include <functional>
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
#include "commands/CEndTurnCommand.h"

namespace Knights {

    CEndTurnCommand::CEndTurnCommand(std::shared_ptr<CGame> aGame) : IGameCommand(aGame) {
    }

    std::string CEndTurnCommand::to_string() const {
        return "Quit";
    }

    bool CEndTurnCommand::shouldEndTurn() {
        return true;
    }

    void CEndTurnCommand::execute() {
    }
}