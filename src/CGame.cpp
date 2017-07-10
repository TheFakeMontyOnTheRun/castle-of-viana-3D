#include <functional>
#include <array>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "Common.h"
#include "CGameSnapshot.h"
#include "CGame.h"

namespace odb {
std::vector<std::tuple<int, int>> visibleCharacters;
std::vector<std::pair<int, int>> visitedCharacters;
    RayCollision CGame::castRay(int offset) {

        const auto limit = 40;

        float rx0 = mCamera.mPosition.mX;
        float ry0 = mCamera.mPosition.mY;

        float rx = rx0;
        float ry = ry0;
        RayCollision collision;
        int angle = wrap360( mCamera.mAngle + offset);

        do {
            rx += sines[ angle ];
            ry += cossines[ angle ];

            if ( mMap[ (ry) ][ (rx) ] < 0 ) {
                float dx = rx - rx0;
                float dy = ry - ry0;

                auto pos = std::pair<int, int>{
                        static_cast<int>(rx),
                        static_cast<int>(ry)
                };

                auto found = std::find( std::begin( visitedCharacters), std::end(visitedCharacters), pos );

                if ( found == std::end( visitedCharacters ) ) {
                    visitedCharacters.push_back( pos );

                    visibleCharacters.emplace_back(static_cast<int>(offset), static_cast<int>(((( dx * dx ) + ( dy * dy )) * cossines[ wrap360( offset)  ] * 16.0f )));
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
        mCamera.mPosition.mX = mCamera.mPosition.mY = 1;

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
                std::array<int, 40>{0,-4,0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
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
    }

    void CGame::tick( long ms ) {

        auto newX = mCamera.mPosition.mX + mCamera.mSpeed.mX;
        auto newY = mCamera.mPosition.mY + mCamera.mSpeed.mY;

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
            mCamera.mPosition.mX = newX;
            mCamera.mPosition.mY = newY;
        } else {
            mCamera.mSpeed = { 0, 0};
        }

        mCamera.mAngle += mCamera.mAngularSpeed;

        mCamera.mSpeed.mX = mCamera.mSpeed.mX / 2.0f;
        mCamera.mSpeed.mY = mCamera.mSpeed.mY / 2.0f;
        mCamera.mAngularSpeed = mCamera.mAngularSpeed / 2.0f;
    }

    CGameSnapshot CGame::getGameSnapshot() {
        CGameSnapshot snapshot;
        visibleCharacters.clear();
        visitedCharacters.clear();

        for (int d = -45; d < 45; ++d) {
            snapshot.mCurrentScan[ d + 45 ] = castRay(d);
        }
        snapshot.mVisibleCharacters = visibleCharacters;
        snapshot.mCamera = mCamera;

        return snapshot;
    }

    CControlCallback CGame::getKeyPressedCallback() {
        return [&](ECommand command) {
            if (command == ECommand::kLeft) {
                mCamera.mAngularSpeed = -5;
            }

            if (command == ECommand::kRight) {
                mCamera.mAngularSpeed = 5;
            }

            mCamera.mAngle = static_cast<int>(mCamera.mAngle) % 360;

            while ( mCamera.mAngle < 0 ) {
                mCamera.mAngle += 360;
            }

            if (command == ECommand::kUp) {
                mCamera.mSpeed.mX = std::sin((mCamera.mAngle * 3.14159f) / 180.0f) * 0.75f;
                mCamera.mSpeed.mY = std::cos((mCamera.mAngle * 3.14159f) / 180.0f) * 0.75f;

            } else if (command == ECommand::kDown) {
                mCamera.mSpeed.mX = -std::sin((mCamera.mAngle * 3.14159f) / 180.0f) * 0.75f;
                mCamera.mSpeed.mY = -std::cos((mCamera.mAngle * 3.14159f) / 180.0f) * 0.75f;
            }
        };
    }

    CControlCallback CGame::getKeyReleasedCallback() {
        return [&](ECommand command) {
        };
    }
}
