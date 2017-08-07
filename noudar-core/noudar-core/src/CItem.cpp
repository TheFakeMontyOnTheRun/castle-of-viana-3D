//
// Created by monty on 12/02/17.
//
#include <iostream>
#include <string>
#include <functional>
#include <memory>

#include "CItem.h"

#include <sstream>

Knights::CItem::CItem(std::string aName, ItemView aView ) : mName(aName), mView( aView ), mConsumable(false) {
}

std::string Knights::CItem::to_string() const {
	return mName;
}

Knights::ItemView Knights::CItem::getView() const {
	return mView;
}

Knights::CItem::CItem(std::string aName, ItemView aView, bool aConsumable, bool aCanBeDropped, const Knights::CItemAction &itemAction) : mName(aName), mView( aView ), mConsumable(aConsumable), mCanBeDropped(aCanBeDropped), mItemAction( itemAction ) {
}

Knights::CItem::CItem(std::string aName, ItemView aView, bool aConsumable, bool aCanBeDropped, const Knights::CItemAction &itemAction, const Knights::CItemAction &itemPickAction, const Knights::CItemAction &itemDropAction) : mName(aName), mView( aView ), mConsumable(aConsumable), mCanBeDropped(aCanBeDropped), mItemAction(itemAction), mItemPickAction(itemPickAction), mItemDropAction(itemDropAction) {
}


void Knights::CItem::use(std::shared_ptr<CActor> aActor, std::shared_ptr<CMap> aMap) {
	mItemAction(aActor, aMap);
}

std::ostream &::Knights::operator<<(std::ostream &os, const Knights::CItem &action) {

	os << action.to_string();

	return os;
}

std::string Knights::to_string(const Knights::CItem &action) {
	return action.to_string();
}

bool Knights::CItem::isConsumable() const {
	return mConsumable;
}

bool Knights::CItem::canBeDropped() const {
    return mCanBeDropped;
}
