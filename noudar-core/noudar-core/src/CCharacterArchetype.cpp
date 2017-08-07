//
// Created by monty on 03/01/17.
//
#include <string>
#include <memory>
#include <functional>
#include <utility>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Vec2i.h"
#include "CTeam.h"
#include "CItem.h"
#include "CActor.h"
#include "CCharacterArchetype.h"

namespace Knights {
	CCharacterArchetype::CCharacterArchetype(int aAttack, int aDefense, int aHP, int aAP, ActorView aView, std::string aName ):
	mAttack(aAttack), mDefense(aDefense), mHP(aHP), mAP(aAP), mView( aView ), mName( aName ) {
	}

	int CCharacterArchetype::getAttack() {
		return mAttack;
	}

	int CCharacterArchetype::getDefense() {
		return mDefense;
	}

	int CCharacterArchetype::getHP() {
		return mHP;
	}

	int CCharacterArchetype::getAP() {
		return mAP;
	}

	ActorView CCharacterArchetype::getView() {
		return mView;
	}

	std::string CCharacterArchetype::getName() {
		return mName;
	}
}
