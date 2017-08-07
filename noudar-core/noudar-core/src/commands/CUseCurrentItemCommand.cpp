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

#include "commands/CUseCurrentItemCommand.h"

Knights::CUseCurrentItemCommand::CUseCurrentItemCommand(std::shared_ptr<Knights::CGame> aGame,
                                                        std::shared_ptr<Knights::CActor> aActor) :
IGameCommand( aGame ), mActor( aActor ) {
}

std::string Knights::CUseCurrentItemCommand::to_string() const {
	return "Use current item";
}

bool Knights::CUseCurrentItemCommand::shouldEndTurn() {
	return true;
}

void Knights::CUseCurrentItemCommand::execute() {

	auto currentItem = mActor->getSelectedItem();

	if ( currentItem != nullptr ) {
		currentItem->use(mActor, getGame()->getMap());
		if ( currentItem->isConsumable() ) {
			mActor->removeItemFromInventory(currentItem);
		}
	}
}
