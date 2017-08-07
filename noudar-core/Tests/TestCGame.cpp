#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <utility>
#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <functional>

#include "Vec2i.h"
#include "CTeam.h"
#include "IMapElement.h"
#include "CItem.h"
#include "CStorageItem.h"
#include "CActor.h"
#include "CCharacterArchetype.h"
#include "IFileLoaderDelegate.h"
#include "Vec2i.h"
#include "IMapElement.h"
#include "CTeam.h"
#include "CActor.h"
#include "CGameDelegate.h"
#include "CMap.h"
#include "IRenderer.h"
#include "CGame.h"


using ::testing::Return;
using ::testing::_;

class MockRenderer : public Knights::IRenderer {
public:

  MOCK_METHOD2(drawMap, void( Knights::CMap &map, std::shared_ptr<Knights::CActor> current ) );
  MOCK_METHOD0(getInput, char());
};

class MockFileLoader : public Knights::IFileLoaderDelegate {
public:

  MOCK_METHOD1( loadFileFromPath, std::string( const std::string& path ) );
  MOCK_METHOD1( loadBinaryFileFromPath, std::vector<char>( const std::string& path ) );
  MOCK_METHOD0( getFilePathPrefix, std::string());
};

class MockActor : public Knights::CActor {
public:
    MockActor( int, int ) : Knights::CActor( 1, 1){ mDefence = 0; mHP =1000; }
    MOCK_METHOD1( performAttack, void( std::shared_ptr<CActor> ) );
};

class TestCGame : public ::testing::Test {
protected:
    std::shared_ptr<MockFileLoader> mMockFileLoader;
    std::shared_ptr<MockRenderer> mMockRenderer;
    std::shared_ptr<Knights::CGameDelegate> mDelegate;
    std::shared_ptr<Knights::CGame> mGame;

    int countElements(char element, std::shared_ptr<Knights::CMap> map ) {
        int count = 0;

        for ( int y = 0; y < Knights::kMapSize; ++y ) {
            for ( int x = 0; x < Knights::kMapSize; ++x ) {
                if ( map->getElementAt( { x, y } ) == element ) {
                    ++count;
                }
            }
        }

        return count;
    }

    void removeAllMonsters() {
        auto actors = mGame->getMap()->getActors();
        auto player = mGame->getMap()->getAvatar();

        for (const auto& actor : actors ) {
            if ( actor != player ) {
                mGame->getMap()->removeActorFrom( actor->getPosition() );
            }
        }
    }

    void removeAllExits() {
        auto map = mGame->getMap();

        for ( int y = 0; y < Knights::kMapSize; ++y ) {
            for ( int x = 0; x < Knights::kMapSize; ++x ) {
                if ( map->getElementAt( { x, y } ) == 'E' ) {
                    map->floodFill({ x, y}, {{ 'E', {'.', Knights::CBlockProperties()}}} );
                }
            }
        }
    }

    int getTotalHealthInLevel() {

        auto actors = mGame->getMap()->getActors();
        auto cumulatedHealth = 0;

        for (const auto& actor : actors ) {
            if ( actor->getView() == '@') {
                cumulatedHealth += actor->getHP();
            }
        }

        return cumulatedHealth;
    }

    std::string getMap() {
        std::string toReturn;

        toReturn += "v4yE00000000000000000A000000000T0000000c\n";
        toReturn += "01111000000000000000+000##TTTTTT000#0000\n";
        toReturn += "050000000000000000000000000000T0000#0#00\n";
        toReturn += "050000000000000000000000000000TTTTTT##00\n";
        toReturn += "000000000000000000000000000000T000000000\n";
        toReturn += "0000000000000000000000000#0000T0000#0000\n";
        toReturn += "0000000000000000000000000#TTTTTTTT##0000\n";
        toReturn += "000000000000000000000000000T000000000000\n";
        toReturn += "000000000000000000000000000#000000000000\n";
        toReturn += "000000000000000000000000000#000000000000\n";
        toReturn += "000000000000000000000000000000000000000w\n";
        toReturn += "6000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0500000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "0000000000000000000000000000000000000000\n";
        toReturn += "d00000000000000000000000000000000000000e\n";
        toReturn += "0000000000000000000000000000000000000000";

        return toReturn;
    }

    virtual void SetUp() {
        mMockFileLoader = std::make_shared<MockFileLoader>();
        mMockRenderer = std::make_shared<MockRenderer>();
        mDelegate = std::make_shared<Knights::CGameDelegate>();
        ON_CALL(*mMockFileLoader, loadFileFromPath(_)).WillByDefault(Return(getMap()));
        mGame = std::make_shared<Knights::CGame>( mMockFileLoader, mMockRenderer, mDelegate );

    }

    // virtual void TearDown() {}
};




TEST_F(TestCGame, GameWillRefreshUponValidMoveTest ) {

  std::string mockMapContents = getMap();

  ON_CALL(*mMockFileLoader, loadFileFromPath(_)).WillByDefault(Return(mockMapContents));
  ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kMovePlayerNorthCommand));
  EXPECT_CALL(*mMockFileLoader, loadFileFromPath(_));
  auto game = std::make_shared<Knights::CGame>( mMockFileLoader, mMockRenderer, mDelegate );
  EXPECT_CALL(*mMockRenderer, drawMap(_,_));
  EXPECT_CALL(*mMockRenderer, getInput());
  game->tick();
}

TEST_F(TestCGame, GameWillKeepPlayerStatusBetweenMapChanges ) {

	mGame->getMap()->getAvatar()->addHP( 50 );
	auto previousHP = mGame->getMap()->getAvatar()->getHP();
	mGame->proceedToNextLevel();
	ASSERT_EQ( previousHP, mGame->getMap()->getAvatar()->getHP() );
    ASSERT_TRUE( Knights::EDirection::kNorth == mGame->getMap()->getAvatar()->getDirection() );
}

TEST_F(TestCGame, GameWillAdvanceLevelUponEnteringExit ) {
    auto actor = mGame->getMap()->getAvatar();
    actor->turnRight();

	ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kMovePlayerForwardCommand));
    mGame->tick();
    EXPECT_CALL(*mMockFileLoader, loadFileFromPath(_));
    mGame->tick();
}

TEST_F(TestCGame, AdvancingLevelWillKeepItemsWorkingProperly ) {
    auto mockMapContents = getMap();
    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kMovePlayerForwardCommand));
    mGame->tick();
    actor->suggestCurrentItem('y');
    EXPECT_CALL(*mMockFileLoader, loadFileFromPath(_));
    //this will load a new level
    mGame->tick();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();
    crossbow->add( 100 );

    auto previousAmmo = crossbow->getAmount();
    mGame->tick();

    auto stillCrossbow = mGame->getMap()->getAvatar()->getItemWithSymbol('y');

    ASSERT_TRUE( stillCrossbow->getView() == crossbow->getView() );
    ASSERT_NE( crossbow->getAmount(), previousAmmo );
}

TEST_F(TestCGame, GameWillInvalidateStoredPlayerForNextSession ) {
	auto avatar = mGame->getMap()->getAvatar();
	avatar->addHP( 50 );
	auto previousHP = avatar->getHP();
	mGame->proceedToNextLevel();
	avatar = mGame->getMap()->getAvatar();
	avatar->addHP(-avatar->getHP() * 2 );
	ASSERT_FALSE( avatar->isAlive() );
	mGame->tick();
	ASSERT_TRUE( mGame->getMap()->getAvatar()->isAlive() );
	ASSERT_NE( previousHP, mGame->getMap()->getAvatar()->getHP() );
}

TEST_F(TestCGame, AvatarInstancesBetweenLevelsAreNotTheSameInstance ) {
	auto mockFileLoader = std::make_shared<MockFileLoader>();
	auto renderer = std::make_shared<MockRenderer>();
	auto delegate = std::make_shared<Knights::CGameDelegate>();
	ON_CALL(*mockFileLoader, loadFileFromPath(_)).WillByDefault(Return(getMap()));
	auto game = std::make_shared<Knights::CGame>( mockFileLoader, renderer, delegate );
	auto avatar = game->getMap()->getAvatar();
	ASSERT_EQ( game->getMap()->getAvatar(), avatar );
	game->proceedToNextLevel();
	ASSERT_NE( game->getMap()->getAvatar(), avatar );
}

TEST_F(TestCGame, RestoredPlayerWillNotRestoreTransientStateFromBackup ) {
	auto mockFileLoader = std::make_shared<MockFileLoader>();
	auto renderer = std::make_shared<MockRenderer>();
	auto delegate = std::make_shared<Knights::CGameDelegate>();
	ON_CALL(*mockFileLoader, loadFileFromPath(_)).WillByDefault(Return(getMap()));
	auto game = std::make_shared<Knights::CGame>( mockFileLoader, renderer, delegate );
	game->getMap()->getAvatar()->turnLeft();
	auto direction = game->getMap()->getAvatar()->getDirection();
	game->proceedToNextLevel();
	game->tick();
	ASSERT_NE( direction, game->getMap()->getAvatar()->getDirection() );
}

TEST_F(TestCGame, GameWillNotTryToLoadFileFromBinaryTest ) {

  auto mockFileLoader = std::make_shared<MockFileLoader>();
  auto renderer = std::make_shared<MockRenderer>();
  auto delegate = std::make_shared<Knights::CGameDelegate>();

  std::string mockMapContents = getMap();
  ON_CALL(*mockFileLoader, loadFileFromPath(_)).WillByDefault(Return(mockMapContents));
  EXPECT_CALL(*mockFileLoader, loadFileFromPath(_));
  std::make_shared<Knights::CGame>( mockFileLoader, renderer, delegate );
}

TEST_F(TestCGame, GameWillPreventPlayersFromDroppingItemsOnInvalidPositions ) {

    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    auto currentItem = actor->getSelectedItem();
    actor->turnRight();
    actor->turnRight();
    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kDropItemCommand));
    mGame->tick();

    auto target = mGame->getMap()->getActorTargetPosition( actor );
    ASSERT_FALSE( mGame->getMap()->isValid(target));
    ASSERT_TRUE( actor->getSelectedItem() != nullptr );
    auto itemOnTheFloor = mGame->getMap()->getItemAt(target);
    ASSERT_TRUE(itemOnTheFloor == nullptr );
}


TEST_F(TestCGame, GameWillPreventPlayersFromPickingItemsOnInvalidPositions ) {

    auto actor = mGame->getMap()->getAvatar();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();

    auto itemBefore = actor->getSelectedItem();

    auto target = mGame->getMap()->getActorTargetPosition( actor );
    ASSERT_FALSE( mGame->getMap()->isValid(target));
    ASSERT_EQ( actor->getSelectedItem(), itemBefore );
    auto itemOnTheFloor = mGame->getMap()->getItemAt(target);
    ASSERT_TRUE(itemOnTheFloor == nullptr );
}

TEST_F(TestCGame, GameWillPreventPlayersFromDroppingItemsOnBlockingTiles ) {

    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    auto currentItem = actor->getSelectedItem();
    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kDropItemCommand));
    mGame->tick();

    auto target = mGame->getMap()->getActorTargetPosition( actor );
    ASSERT_EQ(currentItem->getView(), 'y' );
    auto mapElementView = mGame->getMap()->getElementAt(target);
    ASSERT_EQ(mapElementView, '1' );
    auto itemOnTheFloor = mGame->getMap()->getItemAt(target);
    ASSERT_TRUE(itemOnTheFloor == nullptr );
}

TEST_F(TestCGame, GameWillPreventPlayersFromDroppingItemsOnTopOfOtherItems ) {

    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->turnRight();
    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kDropItemCommand));
    mGame->tick();

    ASSERT_TRUE( actor->getDirection() == Knights::EDirection::kWest);
    auto target = mGame->getMap()->getActorTargetPosition( actor );
    ASSERT_TRUE(actor->getSelectedItem() != nullptr );
    ASSERT_EQ(actor->getSelectedItem()->getView(), 'y' );
    auto mapElementView = mGame->getMap()->getItemViewAt(target);
    Knights::ItemView shield = 'v';
    ASSERT_EQ(mapElementView, shield );
    auto itemOnTheFloor = mGame->getMap()->getItemAt(target);
    auto selectedItem = actor->getSelectedItem();
    ASSERT_TRUE(itemOnTheFloor != selectedItem );
}

TEST_F(TestCGame, HavingBothShieldAndCrossbowChargedWillProvideMoreDamage ) {

    auto actor = mGame->getMap()->getAvatar();
    auto mockEnemy = std::make_shared<MockActor>( 1, 1 );

    mGame->getMap()->addActorAt( mockEnemy, { Knights::kMapSize / 2, 0 } );

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->turnLeft();
    actor->turnLeft();
    mGame->tick();
    actor->turnLeft();
    actor->turnLeft();
    actor->suggestCurrentItem('y');

    auto shield = (Knights::CStorageItem*)actor->getItemWithSymbol( 'v' ).get();
    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();

    shield->add( 100  );
    crossbow->add( 100 );

    auto crossbowAmmoBefore = crossbow->getAmount();
    auto shieldEnergyBefore = shield->getAmount();
    auto healthBefore = mockEnemy->getHP();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    ASSERT_TRUE( actor->getDirection() == Knights::EDirection::kEast);
    ASSERT_EQ(actor->getSelectedItem()->getView(), 'y' );

    auto newHealth = mockEnemy->getHP();

    ASSERT_GT( healthBefore, newHealth );
    ASSERT_TRUE( crossbowAmmoBefore > crossbow->getAmount());
    ASSERT_TRUE( shieldEnergyBefore > shield->getAmount());
}


TEST_F(TestCGame, HavingCrossbowChargedWillProvideRegularDamage ) {

    auto actor = mGame->getMap()->getAvatar();
    auto mockEnemy = std::make_shared<MockActor>( 1, 1 );

    mGame->getMap()->addActorAt( mockEnemy, { Knights::kMapSize / 2, 0 } );


    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->turnLeft();
    actor->turnLeft();
    mGame->tick();
    actor->turnLeft();
    actor->turnLeft();
    actor->suggestCurrentItem('y');

    auto shield = (Knights::CStorageItem*)actor->getItemWithSymbol( 'v' ).get();
    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();

    crossbow->add( 100 );
    shield->empty();

    auto crossbowAmmoBefore = crossbow->getAmount();
    auto shieldEnergyBefore = shield->getAmount();
    auto healthBefore = mockEnemy->getHP();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    ASSERT_TRUE( actor->getDirection() == Knights::EDirection::kEast);
    ASSERT_EQ(actor->getSelectedItem()->getView(), 'y' );

    auto newHealth = mockEnemy->getHP();

    ASSERT_GT( healthBefore, newHealth );
    ASSERT_GT( crossbowAmmoBefore, crossbow->getAmount());
    ASSERT_EQ( shieldEnergyBefore, shield->getAmount());
}

TEST_F(TestCGame, UsingTheShieldWillReplenishTheHealth ) {
    auto actor = mGame->getMap()->getAvatar();

    actor->turnLeft();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->suggestCurrentItem('v');

    auto shield = (Knights::CStorageItem*)actor->getItemWithSymbol( 'v' ).get();
    shield->add( 100 );

    auto shieldEnergyBefore = shield->getAmount();
    auto healthBefore = actor->getHP();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    ASSERT_TRUE( actor->getDirection() == Knights::EDirection::kWest);


    auto newHealth = actor->getHP();

    ASSERT_TRUE( healthBefore < newHealth );
    ASSERT_TRUE( shieldEnergyBefore > shield->getAmount());
}


TEST_F(TestCGame, ShootingTheGateNodeWillOpenAllGates ) {

    auto actor = mGame->getMap()->getAvatar();
    int gatesBefore = countElements('#', mGame->getMap());
    int passagesBefore = countElements('~', mGame->getMap());

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->suggestCurrentItem('y');

    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();

    crossbow->add( 100 );

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();
    mGame->tick();
    int passagesAfter = countElements('~', mGame->getMap());
    int gatesAfter = countElements('#', mGame->getMap());
    int ropesAfter = countElements('T', mGame->getMap());

    ASSERT_TRUE( actor->getDirection() == Knights::EDirection::kEast);
    ASSERT_EQ(actor->getSelectedItem()->getView(), 'y' );

    ASSERT_EQ( gatesBefore, passagesAfter );
    ASSERT_EQ( gatesAfter, 0 );
    ASSERT_EQ( ropesAfter, 0 );
    ASSERT_EQ( passagesBefore, 0 );
}

TEST_F(TestCGame, EnsureMonstersWontKillEachOther ) {

    int cumulatedHealthBefore = getTotalHealthInLevel();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kEndTurnCommand));
    mGame->tick();
    mGame->tick();
    mGame->tick();

    ASSERT_EQ( getTotalHealthInLevel(), cumulatedHealthBefore);
}

TEST_F(TestCGame, EnsureTheProjectionTargetWillChangeBasedOnCharacterItems ) {
    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    auto targetBeforeCrossbow = mGame->getMap()->getTargetProjection( actor );
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->suggestCurrentItem('y');
    auto targetWithCrossbow = mGame->getMap()->getTargetProjection( actor );
    actor->turnRight();
    actor->turnRight();
    mGame->tick();
    actor->suggestCurrentItem('t');
    auto targetWithSword  = mGame->getMap()->getTargetProjection( actor );

    ASSERT_EQ( targetBeforeCrossbow, Knights::Vec2i( 2, 0 ) );
    ASSERT_EQ(targetWithCrossbow, Knights::Vec2i( 31, 0 ) );
    ASSERT_EQ( targetWithSword, Knights::Vec2i( 0, 0 ) );

}

TEST_F(TestCGame, ShootingWallsWillBlockDarts ) {
    auto actor = mGame->getMap()->getAvatar();
    int cumulatedHealthBefore = getTotalHealthInLevel();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->suggestCurrentItem('y');
    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();
    crossbow->add( 100 );
    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    ASSERT_TRUE( actor->getDirection() == Knights::EDirection::kSouth);
    ASSERT_EQ(actor->getSelectedItem()->getView(), 'y' );
    ASSERT_EQ( getTotalHealthInLevel(), cumulatedHealthBefore);
}

TEST_F(TestCGame, KillingTheCocoonWillSpawnTheWeakenedMasterDemon ) {
    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->suggestCurrentItem('y');
    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();
    crossbow->add( 100 );

    //clear the path to the cocoon
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    //kill the cocoon
    auto shouldBeCocoonNow = mGame->getMap()->getActorAt({ Knights::kMapSize - 1, 0 })->getView();

    mGame->tick();
    mGame->tick();
    auto shouldBeDemonNow= mGame->getMap()->getActorAt({ Knights::kMapSize - 1, 0 })->getView();

    ASSERT_EQ( shouldBeCocoonNow, 'C');
    ASSERT_EQ( shouldBeDemonNow, 'd');
}

TEST_F(TestCGame, CocoonShouldNotMove ) {
    auto actor = mGame->getMap()->getAvatar();

    auto shouldBeCocoonNow = mGame->getMap()->getActorAt({ Knights::kMapSize - 1, 0 })->getView();
    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize - 2, 0 }, actor );
    actor->turnRight();
    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kMovePlayerForwardCommand));
    mGame->tick();
    mGame->tick();
    auto shouldStillBeCocoon = mGame->getMap()->getActorAt({ Knights::kMapSize - 1, 0 })->getView();

    ASSERT_EQ( shouldBeCocoonNow, 'C');
    ASSERT_EQ( shouldStillBeCocoon, 'C');
}

TEST_F(TestCGame, WeakDemonShouldNotBeVulnerableToSwords ) {
    auto actor = mGame->getMap()->getAvatar();

    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize / 2 , Knights::kMapSize - 2 }, actor );
    actor->turnRight();
    actor->turnRight();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->suggestCurrentItem('t');
    actor->turnRight();
    mGame->getMap()->moveActor( actor->getPosition(), { 1, Knights::kMapSize - 2 }, actor );

    auto weakDemonHealthBefore = mGame->getMap()->getActorAt( { 0, Knights::kMapSize - 2 })->getHP();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    auto weakDemonHealthAfter = mGame->getMap()->getActorAt( { 0, Knights::kMapSize - 2 })->getHP();

    ASSERT_EQ( weakDemonHealthAfter, weakDemonHealthBefore );
}


TEST_F(TestCGame, WeakDemonShouldNotBeVulnerableToCrossbows ) {
    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->suggestCurrentItem('y');
    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();
    crossbow->add( 100 );

    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize / 2 , Knights::kMapSize - 2 }, actor );
    actor->turnRight();
    actor->turnRight();
    auto weakDemonEnergyBeforeBolt = mGame->getMap()->getActorAt( { 0, Knights::kMapSize - 2 })->getHP();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    auto weakDemonEnergyAfterBolt = mGame->getMap()->getActorAt( { 0, Knights::kMapSize - 2 })->getHP();

    ASSERT_EQ( weakDemonEnergyBeforeBolt, weakDemonEnergyAfterBolt );
}

TEST_F(TestCGame, StrongDemonShouldNotBeVulnerableToSwords ) {
    auto actor = mGame->getMap()->getAvatar();

    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize / 2 , Knights::kMapSize - 2 }, actor );
    actor->turnRight();
    actor->turnRight();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->suggestCurrentItem('t');
    actor->turnLeft();
    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize - 2, Knights::kMapSize - 2 }, actor );

    auto strongDemonHealthBefore = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize - 2 })->getHP();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    auto strongDemonHealthAfter = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize - 2 })->getHP();

    ASSERT_EQ( strongDemonHealthAfter, strongDemonHealthBefore );
}

TEST_F(TestCGame, StrongDemonShouldNotBeVulnerableToCrossbows ) {
    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->suggestCurrentItem('y');
    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();
    crossbow->add( 100 );

    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize / 2 , Knights::kMapSize - 2 }, actor );

    auto strongDemonEnergyBeforeBolt = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize - 2 })->getHP();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    auto strongDemonEnergyAfterBolt = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize - 2 })->getHP();

    ASSERT_EQ( strongDemonEnergyBeforeBolt, strongDemonEnergyAfterBolt );
}


TEST_F(TestCGame, StrongDemonShouldNotBeVulnerableToCrossbowsAndDischardgedShield ) {
    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();

    actor->turnRight();
    actor->turnRight();
    mGame->tick();
    actor->turnLeft();
    actor->turnLeft();

    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize / 2 , Knights::kMapSize - 2 }, actor );

    auto strongDemonEnergyBeforeBolt = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize - 2 })->getHP();
    auto shield = (Knights::CStorageItem*)actor->getItemWithSymbol( 'v' ).get();
    shield->add( -shield->getAmount() );
    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();
    crossbow->add( 100 );

    actor->suggestCurrentItem('y');

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    auto strongDemonEnergyAfterBolt = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize - 2 })->getHP();

    ASSERT_EQ( strongDemonEnergyBeforeBolt, strongDemonEnergyAfterBolt );
}

TEST_F(TestCGame, EvilSpiritsShouldNotBeVulnerableToSwords ) {
    auto actor = mGame->getMap()->getAvatar();

    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize / 2 , Knights::kMapSize - 2 }, actor );
    actor->turnRight();
    actor->turnRight();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();
    actor->suggestCurrentItem('t');
    actor->turnLeft();
    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize - 2, Knights::kMapSize / 4 }, actor );

    auto strongDemonHealthBefore = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize / 4 })->getHP();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    auto strongDemonHealthAfter = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize / 4 })->getHP();

    ASSERT_EQ( strongDemonHealthAfter, strongDemonHealthBefore );
}


TEST_F(TestCGame, EvilSpiritShouldBeVulnerableToTheCrossbow ) {
    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();

    actor->turnRight();
    actor->turnRight();
    mGame->tick();
    actor->turnLeft();
    actor->turnLeft();

    mGame->getMap()->moveActor( actor->getPosition(), { 0, Knights::kMapSize / 4 }, actor );

    auto strongDemonEnergyBeforeBolt = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize / 4 })->getHP();
    auto shield = (Knights::CStorageItem*)actor->getItemWithSymbol( 'v' ).get();
    shield->add( -shield->getAmount() );
    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();
    crossbow->add( 100 );

    actor->suggestCurrentItem('y');

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    auto strongDemonEnergyAfterBolt = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize / 4 })->getHP();

    ASSERT_GT( strongDemonEnergyBeforeBolt, strongDemonEnergyAfterBolt );
}


TEST_F(TestCGame, StrongDemonShouldBeVulnerableToCrossbowsAndChardgedShield ) {
    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();

    actor->turnRight();
    actor->turnRight();
    mGame->tick();
    actor->turnLeft();
    actor->turnLeft();

    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize / 2 , Knights::kMapSize - 2 }, actor );

    auto strongDemonEnergyBeforeBolt = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize - 2 })->getHP();
    auto shield = (Knights::CStorageItem*)actor->getItemWithSymbol( 'v' ).get();
    shield->add( -shield->getAmount() + 20 );
    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();
    crossbow->add( 100 );

    actor->suggestCurrentItem('y');

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    auto strongDemonEnergyAfterBolt = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize - 2 })->getHP();

    ASSERT_GT( strongDemonEnergyBeforeBolt, strongDemonEnergyAfterBolt );
}

TEST_F(TestCGame, WeakDemonShouldNotBeVulnerableToCrossbowsAndChardgedShield ) {
    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();

    actor->turnRight();
    actor->turnRight();
    mGame->tick();

    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize / 2 , Knights::kMapSize - 2 }, actor );

    auto weakDemonEnergyBeforeBolt = mGame->getMap()->getActorAt( { 0, Knights::kMapSize - 2 })->getHP();
    auto shield = (Knights::CStorageItem*)actor->getItemWithSymbol( 'v' ).get();
    shield->add( -shield->getAmount() + 20 );
    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();
    crossbow->add( 100 );

    actor->suggestCurrentItem('y');

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    auto weakDemonEnergyAfterBolt = mGame->getMap()->getActorAt( { 0, Knights::kMapSize - 2 })->getHP();

    ASSERT_EQ( weakDemonEnergyBeforeBolt, weakDemonEnergyAfterBolt );
}

TEST_F(TestCGame, WeakDemonShouldHaveAHalfMapViewRange ) {
    auto actor = mGame->getMap()->getAvatar();

    //add one turn here so that if the demon master had any extra range, he would still pursue the player from this far
    //and causing it not to be on the predicted position down below
    mGame->tick();

    mGame->getMap()->moveActor( actor->getPosition(), { (Knights::kMapSize / 4), Knights::kMapSize - 2 }, actor );
    auto monsterAtOriginalPosition = mGame->getMap()->getActorAt({ 0, Knights::kMapSize - 2 });
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kEndTurnCommand));
    mGame->tick();

    ASSERT_TRUE( monsterAtOriginalPosition != nullptr );
    ASSERT_EQ( monsterAtOriginalPosition->getPosition(), Knights::Vec2i( 1, Knights::kMapSize - 2 ) );
}

TEST_F(TestCGame, StrongDemonShouldHaveAHalfMapViewRange ) {
    auto actor = mGame->getMap()->getAvatar();

    //add one turn here so that if the demon master had any extra range, he would still pursue the player from this far
    //and causing it not to be on the predicted position down below
    mGame->tick();

    mGame->getMap()->moveActor( actor->getPosition(), { ( (3 * Knights::kMapSize) / 4), Knights::kMapSize - 2 }, actor );
    auto monsterAtOriginalPosition = mGame->getMap()->getActorAt({ Knights::kMapSize - 1, Knights::kMapSize - 2 });
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kEndTurnCommand));
    mGame->tick();

    ASSERT_TRUE( monsterAtOriginalPosition != nullptr );
    ASSERT_EQ( monsterAtOriginalPosition->getPosition(), Knights::Vec2i( Knights::kMapSize - 2, Knights::kMapSize - 2 ) );
}

TEST_F(TestCGame, KillingTheMasterDemonWillSpawnEndLevelPortal ) {

    auto actor = mGame->getMap()->getAvatar();
    auto demon = mGame->getMap()->getActorAt( { Knights::kMapSize - 1, Knights::kMapSize - 2 });
    demon->addHP( 1 - demon->getHP() );

    actor->turnRight();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();

    actor->turnRight();

    actor->turnRight();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();

    actor->turnLeft();

    actor->turnLeft();

    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize / 2 , Knights::kMapSize - 2 }, actor );

    auto shield = (Knights::CStorageItem*)actor->getItemWithSymbol( 'v' ).get();
    shield->add( -shield->getAmount() + 20 );

    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();
    crossbow->add( 100 );

    actor->suggestCurrentItem('y');

    ASSERT_TRUE( demon->isAlive() );

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    removeAllMonsters();
    removeAllExits();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kEndTurnCommand));
    EXPECT_CALL(*mMockFileLoader, loadFileFromPath(_));
    mGame->tick();
}

TEST_F(TestCGame, TakingATokenOfFaithWillReplenishHealth ) {
    auto actor = mGame->getMap()->getAvatar();

    auto playerOriginalHealth = actor->getHP();
    mGame->tick();
    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize / 2, 0 }, actor );

    //get the token
    actor->turnLeft();
    actor->turnLeft();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();

    auto healthAfterPickingIt = actor->getHP();

    //wait several turns...
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kEndTurnCommand));
    mGame->tick();
    mGame->tick();
    mGame->tick();
    mGame->tick();
    auto healthAfterSeveralTurnsHoldingIt = actor->getHP();

    //finally use it
    actor->suggestCurrentItem('+');
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();

    auto turn1InEffect = actor->getHP();
    mGame->tick();

    auto noLongerInEffect = actor->getHP();
    mGame->tick();

    auto stopTryingItWontWork = actor->getHP();
    mGame->tick();

    auto pleaseRefrainFromInsistingItsNotGoingToWorkAndWillOnlyHumiliateYourself = actor->getHP();
    mGame->tick();

    //some more turns
    mGame->tick();
    mGame->tick();
    mGame->tick();
    mGame->tick();

    auto finalHealth = actor->getHP();

    ASSERT_EQ( healthAfterPickingIt, playerOriginalHealth );
    ASSERT_EQ( healthAfterSeveralTurnsHoldingIt, playerOriginalHealth );
    ASSERT_EQ( turn1InEffect, playerOriginalHealth + 20 );
    ASSERT_EQ( noLongerInEffect, playerOriginalHealth + 20  );
    ASSERT_EQ( stopTryingItWontWork, playerOriginalHealth + 20  );
    ASSERT_EQ( pleaseRefrainFromInsistingItsNotGoingToWorkAndWillOnlyHumiliateYourself, playerOriginalHealth + 20 );
    ASSERT_EQ( finalHealth, playerOriginalHealth + 20 );
}

TEST_F(TestCGame, PlayerShouldNotBeAbleToDropEssentialWeapons ) {
    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kDropItemCommand));
    mGame->tick();

    ASSERT_NE( actor->getSelectedItem(), nullptr );
}

TEST_F(TestCGame, PlayerShouldStartWithSword ) {
    ASSERT_EQ( mGame->getMap()->getAvatar()->getSelectedItem()->getView(), 't' );
}

TEST_F(TestCGame, DeadMonksShouldDropTokensOfFaith ) {
    auto actor = mGame->getMap()->getAvatar();

    actor->turnRight();
    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();

    actor->turnRight();
    actor->turnRight();
    mGame->tick();
    actor->turnLeft();
    actor->turnLeft();

    mGame->getMap()->moveActor( actor->getPosition(), { Knights::kMapSize / 2, (Knights::kMapSize / 2) + 1}, actor );
    actor->setDirection(Knights::EDirection::kWest);
    auto crossbow = (Knights::CStorageItem*)actor->getItemWithSymbol( 'y' ).get();
    crossbow->add( 100 );

    actor->suggestCurrentItem('y');

    mGame->getMap()->moveActor( actor->getPosition(), { (Knights::kMapSize / 2), (Knights::kMapSize / 4) + 1}, actor );

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kUseCurrentItemInInventoryCommand));
    mGame->tick();
    mGame->tick();

    mGame->getMap()->moveActor( actor->getPosition(), { 1, (Knights::kMapSize / 4) + 1}, actor );

    ON_CALL(*mMockRenderer, getInput()).WillByDefault(Return(Knights::kPickItemCommand));
    mGame->tick();

    actor->suggestCurrentItem('+');

    ASSERT_EQ( actor->getSelectedItem()->getView(), '+');
}