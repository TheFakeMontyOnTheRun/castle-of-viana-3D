#include <functional>
#include <string>
#include <memory>
#include <utility>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Vec2i.h"
#include "CTeam.h"
#include "CItem.h"
#include "CActor.h"

namespace Knights {
	CActor::CActor(ActorId aId, int defaultAP) :
			mStance(EStance::kStanding), mDirection(EDirection::kNorth), mId(aId), mMoves(0), mDefaultAP(defaultAP),
			mRemainingAP(defaultAP), mAttack(0), mDefence(0), mHP(0) {
	}

	bool CActor::canMove() {
		return hasEnoughAP();
	}

	bool CActor::canAttack() {
		return hasEnoughAP();
	}

	bool CActor::hasEnoughAP() {
		return mRemainingAP > 0;
	}

	void CActor::onMove() {
		--mRemainingAP;
		++mMoves;
		mStance = EStance::kAttacking;
	}

	void CActor::onAttack() {
		--mRemainingAP;
	}

	void CActor::turnLeft() {

		if (mDirection == EDirection::kNorth) {
			mDirection = EDirection::kWest;
		} else {
			mDirection = static_cast<EDirection>( static_cast<int>(mDirection) - 1);
		}
	}

	void CActor::turnRight() {
		if (mDirection == EDirection::kWest) {
			mDirection = EDirection::kNorth;
		} else {
			mDirection = static_cast<EDirection>( static_cast<int>(mDirection) + 1);
		}
	}

	void CActor::endOfTurn() {
		mRemainingAP = mDefaultAP;
		mStance = EStance::kStanding;
	}

	void CActor::performAttack(std::shared_ptr<CActor> other) {

		onAttack();
		int diff = (getAttack() - other->getDefense());

		if (diff > 0) {
            other->addHP(-diff );
		}
	}

	Vec2i CActor::getPosition() {
		return mPosition;
	}

	void CActor::setPosition(Vec2i position) {
		mPosition = position;
	}

	std::shared_ptr<CTeam> CActor::getTeam() {
		return mTeam;
	}

	bool CActor::isAlive() {
		return mHP > 0;
	}

	EDirection CActor::getDirection() {
		return mDirection;
	}

	int CActor::getHP() {
		return mHP;
	}

	int CActor::getAP() {
		return mRemainingAP;
	}

	int CActor::getDefense() {
		return mDefence;
	}

	int CActor::getAttack() {
		return mAttack + mAttackBonus;
	}

	ActorId CActor::getId() {
		return mId;
	}

	void CActor::setDirection(EDirection d) {
		mDirection = d;
	}

	std::string CActor::getName() {
		return mName;
	}

	ActorView CActor::getView() {
		return mView;
	}

	int CActor::getMoves() {
		return mMoves;
	}


	void CActor::addHP(int aHP) {
		mHP += aHP;
	}

	EStance CActor::getStance() {
		return mStance;
	}

	std::ostream &operator<<(std::ostream &os, const EStance &aStance) {
		os << to_string(aStance);

		return os;
	}

	std::string to_string(const EStance &aStance) {
		switch (aStance) {
			case EStance::kAttacking:
				return "Attacking";
			case EStance::kDead:
				return "Dead";
			case EStance::kStanding:
			default:
				return "Standing";
		}
	}

	void CActor::selectNextItem() {

		if ( mInventory.empty() ) {
			return;
		}

		auto it = std::find(std::begin(mInventory), std::end(mInventory), mCurrentItem);

		auto nextIt = std::next( it );
		if ( nextIt != std::end( mInventory ) ) {
			mCurrentItem = *nextIt;
		} else {
			mCurrentItem = *std::begin(mInventory);
		}
	}

	void CActor::selectPreviousItem() {
		auto it = std::find(std::begin(mInventory), std::end(mInventory), mCurrentItem);
		if (mCurrentItem != *std::begin(mInventory)) {
			mCurrentItem = *std::prev(it);
		} else {
			mCurrentItem = *std::prev(std::end(mInventory));
		}
	}

	std::shared_ptr<CItem> CActor::getSelectedItem() {
		return mCurrentItem;
	}

	void CActor::useCurrentItem() {
		this->selectNextItem();
		mHP++;
	}

	void CActor::giveItem(std::shared_ptr<CItem> aItem) {

		if ( aItem == nullptr ) {
			return;
		}

		mInventory.push_back(aItem);
		mCurrentItem = aItem;
	}

	std::shared_ptr<CItem> CActor::removeItemFromInventory(std::shared_ptr<CItem> itemToRemove) {

		auto it = std::find(std::begin(mInventory), std::end(mInventory), itemToRemove);
		mInventory.erase(it);

		if (mInventory.empty()) {
			mCurrentItem = nullptr;
		} else {
			mCurrentItem = *std::begin(mInventory);
		}

		return itemToRemove;
	}

	std::string CActor::getCurrentSay() {
		return mCurrentSay;
	}

	void CActor::setCurrentSay(std::string newSay) {
		mCurrentSay = newSay;
	}

	void CActor::copyStateFrom(std::shared_ptr<CActor> other) {
		this->mName = other->mName;
		this->mAttack = other->mAttack;
		this->mCurrentItem = other->mCurrentItem;
		this->mCurrentSay = other->mCurrentSay;
		this->mDefaultAP = other->mDefaultAP;
		this->mView = other->mView;
		this->mDefence = other->mDefence;
		this->mStance = other->mStance;
		this->mTeam = other->mTeam;
		this->mHP = other->mHP;
		this->mInventory = other->mInventory;
	}

    void CActor::suggestCurrentItem( ItemView view ) {
        auto item = getItemWithSymbol( view );

        if ( item != nullptr ) {
            mCurrentItem = item;
        }
    }

    std::shared_ptr<CItem> CActor::getItemWithSymbol(ItemView symbol) {

		for ( const auto& item : mInventory ) {
			if ( item->getView() == symbol ) {
				return item;
			}
		}

		return nullptr;
    }

    void CActor::setAttackBonus(int attackBonus) {
        mAttackBonus = attackBonus;
    }
}
