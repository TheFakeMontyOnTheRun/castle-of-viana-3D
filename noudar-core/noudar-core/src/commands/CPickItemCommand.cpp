//
// Created by monty on 13/02/17.
//
#include <array>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <functional>
#include "Vec2i.h"
#include "IMapElement.h"
#include "CGameDelegate.h"
#include <string>
#include <memory>
#include <utility>
#include <iostream>
#include <vector>
#include <map>
#include "Vec2i.h"
#include "CTeam.h"
#include "CItem.h"
#include "CActor.h"


#include "CMap.h"
#include "IRenderer.h"
#include "IFileLoaderDelegate.h"
#include "CPlainFileLoader.h"

#include "CGame.h"
#include "commands/IGameCommand.h"
#include "commands/CPickItemCommand.h"

Knights::CPickItemCommand::CPickItemCommand(std::shared_ptr<Knights::CGame> aGame,
                                            std::shared_ptr<Knights::CActor> aActor) : IGameCommand( aGame ), mActor(aActor) {
}

std::string Knights::CPickItemCommand::to_string() const {
	return "Pick item";
}

bool Knights::CPickItemCommand::shouldEndTurn() {
	return true;
}

void Knights::CPickItemCommand::execute() {
	auto map = getGame()->getMap();
	map->giveItemAt( map->getActorTargetPosition(mActor), mActor );
}
