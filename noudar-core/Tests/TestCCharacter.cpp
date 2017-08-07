#include "gtest/gtest.h"
#include "gmock/gmock.h"

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

class MockCharacter : public Knights::CCharacter {
 public:
  MockCharacter( std::shared_ptr<Knights::CCharacterArchetype> aArchetype, std::shared_ptr<Knights::CTeam> aTeam, int aId) : CCharacter( aArchetype, aTeam, aId ) {
  }

  MOCK_METHOD0( hasEnoughAP, bool() );
};

TEST(TestCCharacter, TestArchetypeInheritance ) {
  auto team1 = std::make_shared<Knights::CTeam>("Test");
  auto archetype1 = std::make_shared<Knights::CCharacterArchetype>( 1, 2, 3, 4, '@', "TestCharacter" );
  int id1 = 0;
  auto actor1 = std::make_shared<Knights::CCharacter>( archetype1, team1, id1 );
  
  EXPECT_EQ( 1, actor1->getAttack() );
}

TEST(TestCCharacter, TestCharactersOfSameTeamCantAttackEachOther ) {
  auto team1 = std::make_shared<Knights::CTeam>("Test");
  auto archetype1 = std::make_shared<Knights::CCharacterArchetype>( 1, 2, 3, 4, '@', "TestCharacter" );
  auto actor1 = std::make_shared<Knights::CCharacter>( archetype1, team1, 1 );
  auto actor2 = std::make_shared<Knights::CCharacter>( archetype1, team1, 2 );



  auto healthBefore = actor2->getHP();
  actor1->performAttack( actor2 );
  auto healthAfter = actor2->getHP();

  EXPECT_NE( actor1, actor2 );
  EXPECT_EQ( healthBefore, healthAfter );
}

