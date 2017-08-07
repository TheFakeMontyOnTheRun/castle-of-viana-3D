#include <string>
#include <vector>
#include <cstdlib>
#include <memory>
#include <cmath>
#include <functional>
#include "Vec2i.h"
#include "CTeam.h"
#include "IMapElement.h"
#include "CItem.h"
#include "CActor.h"
#include "CCharacterArchetype.h"
#include "CCharacter.h"

namespace Knights {

	CCharacter::CCharacter(std::shared_ptr<Knights::CCharacterArchetype> aArchetype, std::shared_ptr<CTeam> aTeam, ActorId aId, CUpdateMethod aUpdateMethod) :
	CActor( aId, aArchetype->getAP() ), mUpdateMethod( aUpdateMethod ), mArchetype( *aArchetype ) {
        mView = aArchetype->getView();
        mTeam = aTeam;
        mHP = aArchetype->getHP();
        mAttack = aArchetype->getAttack();
        mDefence = aArchetype->getDefense();
        mName = aArchetype->getName();
	}

    void CCharacter::update(std::shared_ptr<CMap> map) {
        CActor::update(map);

        mUpdateMethod(shared_from_this(), map );
    }

    CCharacterArchetype CCharacter::getArchetype() {
        return mArchetype;
    }
}
