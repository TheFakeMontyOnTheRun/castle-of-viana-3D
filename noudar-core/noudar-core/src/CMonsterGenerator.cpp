//
// Created by monty on 29-03-2017.
//
#include <array>
#include <string>
#include <vector>
#include <cstdlib>
#include <memory>
#include <cmath>
#include <map>
#include <functional>
#include "Vec2i.h"
#include "IMapElement.h"
#include "CTeam.h"
#include "CItem.h"
#include "CActor.h"
#include "CCharacterArchetype.h"
#include "CCharacter.h"
#include "CMonster.h"
#include "CGameDelegate.h"
#include "CMap.h"

#include "CMonsterGenerator.h"

Knights::CMonsterGenerator::CMonsterGenerator(std::shared_ptr<CCharacterArchetype> aArchetypeToBuild, std::shared_ptr<CTeam> aTeam,
ActorId aId, int aliveForTurns) : CActor(aId, 1), mArchetypeToBuild(aArchetypeToBuild), mTeam(aTeam) {
    mHP = aliveForTurns;
}

void Knights::CMonsterGenerator::update(std::shared_ptr<Knights::CMap> map) {
    CActor::update(map);

    --mHP;
    auto position = getPosition();
    auto id = map->getLastestId();

    if ( map->getActorAt( { position.x, position.y - 1 } ) == nullptr ) {
        map->addActorAt( std::make_shared<CMonster>( mArchetypeToBuild, mTeam, id, 8), { position.x, position.y - 1 }  );
    } else if ( map->getActorAt( { position.x, position.y + 1 } ) == nullptr ) {
        map->addActorAt( std::make_shared<CMonster>( mArchetypeToBuild, mTeam, id, 8), { position.x, position.y + 1 }  );
    } else if ( map->getActorAt( { position.x - 1, position.y } ) == nullptr ) {
        map->addActorAt( std::make_shared<CMonster>( mArchetypeToBuild, mTeam, id, 8), { position.x - 1, position.y - 1 }  );
    } else if ( map->getActorAt( { position.x + 1, position.y } ) == nullptr ) {
        map->addActorAt( std::make_shared<CMonster>( mArchetypeToBuild, mTeam, id, 8), { position.x + 1, position.y - 1 }  );
    }
}
