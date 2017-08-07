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

#include "commands/CDropItemCommand.h"

Knights::CDropItemCommand::CDropItemCommand(std::shared_ptr<Knights::CGame> aGame,
                                            std::shared_ptr<Knights::CActor> aActor) : IGameCommand( aGame ), mActor( aActor ) {
}

std::string Knights::CDropItemCommand::to_string() const {
	return "Drop item";
}

bool Knights::CDropItemCommand::shouldEndTurn() {
	return true;
}

void Knights::CDropItemCommand::execute() {
	auto itemToDrop = mActor->getSelectedItem();

	if ( itemToDrop == nullptr ) {
		return;
	}

	if ( !itemToDrop->canBeDropped() ) {
        return;
	}

	auto map = getGame()->getMap();
	auto target = map->getActorTargetPosition(mActor);

	if (map->isBlockMovementAt(target)) {
		return;
	}

	if (map->getItemAt( target ) != nullptr ) {
		return;
	}

	map->putItemAt( mActor->removeItemFromInventory(itemToDrop), target );
}
