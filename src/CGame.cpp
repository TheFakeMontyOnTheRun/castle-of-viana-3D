#include <functional>
#include <array>
#include <memory>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "Common.h"
#include "CGameSnapshot.h"
#include "CGame.h"

namespace odb {
std::vector<std::tuple<int, int, int>> visibleCharacters;
std::vector<std::pair<int, int>> visitedCharacters;
    RayCollision CGame::castRay(int offset) {

        const auto limit = 40;

        float rx0 = mCamera->mPosition.mX;
        float ry0 = mCamera->mPosition.mY;

        float rx = rx0;
        float ry = ry0;
        RayCollision collision;
        int angle = wrap360( mCamera->mAngle + offset);

        do {
            rx += sines[ angle ];
            ry += cossines[ angle ];
            auto type = mMap[ (ry) ][ (rx) ];
            if ( type < 0 ) {
                float dx = rx - rx0;
                float dy = ry - ry0;

                auto pos = std::pair<int, int>{
                        static_cast<int>(rx),
                        static_cast<int>(ry)
                };

                auto found = std::find( std::begin( visitedCharacters), std::end(visitedCharacters), pos );

                if ( found == std::end( visitedCharacters ) ) {
                    visitedCharacters.push_back( pos );

                    visibleCharacters.emplace_back(static_cast<int>(offset), static_cast<int>(((( dx * dx ) + ( dy * dy )) * cossines[ wrap360( offset)  ] * 16.0f )), type );
                }


            }

        } while ( ( rx > 0 && rx < limit && ry > 0 && ry < limit ) && ( mMap[ ry ][ rx ] <= 0 ));


        float dx = rx - rx0;
        float dy = ry - ry0;


        collision.mSquaredDistance = ((( dx * dx ) + ( dy * dy )) * cossines[ wrap360( offset)  ] * 16.0f );
        collision.mCollisionPoint = { rx, ry };
        collision.mHeight = 1;
        collision.mElement = mMap[ ry ][ rx ];
        return collision;
    }



    CGame::CGame() {

        for ( int c = 0; c < 360; ++c ) {
            auto sin_a = (std::sin((c * 3.14159f) / 180.0f)) / 16.0f;
            auto cos_a = (std::cos((c * 3.14159f) / 180.0f)) / 16.0f;
            sines[ c ] = sin_a;
            cossines[ c ] = cos_a;
        }

        mMap = std::array<std::array<int, 40>, 40>{
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,2,0, 0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,3,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0},
                std::array<int, 40>{0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0},
                std::array<int, 40>{0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0},
                std::array<int, 40>{0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                std::array<int, 40>{0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0},
                std::array<int, 40>{0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
                std::array<int, 40>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
        };

        mCamera = std::make_shared<CActor>();
        mCamera->mPosition = {1,1};
        mActors.push_back(mCamera);

        auto foe = std::make_shared<CActor>();
        foe->mPosition = {2,1};
        foe->mSpeed.mY = 4.0f;
        mActors.push_back(foe);
    }

    void CGame::tick( long ms ) {

        for ( int y = 0; y < 40; ++y ) {
            for ( int x = 0; x < 40; ++x ) {
                if ( mMap[ y ][ x ] < 0 ) {
                    mMap[ y ][ x ] = 0;
                }
            }
        }

        for ( auto& actor : mActors ) {

            auto newX = actor->mPosition.mX + actor->mSpeed.mX;
            auto newY = actor->mPosition.mY + actor->mSpeed.mY;

            if (newX >= 40) {
                newX = 39;
            }

            if (newY >= 40) {
                newY = 39;
            }

            if (newX < 0) {
                newX = 0;
            }

            if (newY < 0) {
                newY = 0;
            }

            if (mMap[newY ][ newX ] == 0 ) {
                actor->mPosition.mX = newX;
                actor->mPosition.mY = newY;
            } else {
                actor->mSpeed = { 0, 0};
            }

            actor->mAngle += actor->mAngularSpeed;

            if (actor->mType == EActorType::kEnemy) {
                actor->mSpeed.mX = actor->mSpeed.mX / 2.0f;
                actor->mSpeed.mY = actor->mSpeed.mY / 2.0f;
                actor->mAngularSpeed = actor->mAngularSpeed / 2.0f;
            }

            if ( actor != mCamera ) {
                mMap[newY][newX] = (actor->mType == EActorType::kEnemy) ? -4 : -5;
            }
        }
    }

    CGameSnapshot CGame::getGameSnapshot() {
        CGameSnapshot snapshot;
        visibleCharacters.clear();
        visitedCharacters.clear();

        for (int d = -45; d < 45; ++d) {
            snapshot.mCurrentScan[ d + 45 ] = castRay(d);
        }
        snapshot.mVisibleCharacters = visibleCharacters;
        snapshot.mCamera = *mCamera;

        return snapshot;
    }

    void CGame::spawnFireball(int x, int y, float angle) {
        auto foe = std::make_shared<CActor>();
        foe->mType = EActorType::kFireball;
        foe->mPosition = {x,y};
        foe->mSpeed = { std::sin((angle * 3.14159f) / 180.0f), std::cos((mCamera->mAngle * 3.14159f) / 180.0f) };
        mActors.push_back(foe);
    }

    CControlCallback CGame::getKeyPressedCallback() {
        return [&](ECommand command) {

            if (command == ECommand::kFire1) {
                spawnFireball( round(std::sin((mCamera->mAngle * 3.14159f) / 180.0f) * 0.75f), round(std::cos((mCamera->mAngle * 3.14159f) / 180.0f) * 0.75f), mCamera->mAngle );
            }

            if (command == ECommand::kLeft) {
                mCamera->mAngularSpeed = -5;
            }

            if (command == ECommand::kRight) {
                mCamera->mAngularSpeed = 5;
            }

            mCamera->mAngle = static_cast<int>(mCamera->mAngle) % 360;

            while ( mCamera->mAngle < 0 ) {
                mCamera->mAngle += 360;
            }

            if (command == ECommand::kUp) {
                mCamera->mSpeed.mX = std::sin((mCamera->mAngle * 3.14159f) / 180.0f) * 0.75f;
                mCamera->mSpeed.mY = std::cos((mCamera->mAngle * 3.14159f) / 180.0f) * 0.75f;

            } else if (command == ECommand::kDown) {
                mCamera->mSpeed.mX = -std::sin((mCamera->mAngle * 3.14159f) / 180.0f) * 0.75f;
                mCamera->mSpeed.mY = -std::cos((mCamera->mAngle * 3.14159f) / 180.0f) * 0.75f;
            }
        };
    }

    CControlCallback CGame::getKeyReleasedCallback() {
        return [&](ECommand command) {
        };
    }
}
