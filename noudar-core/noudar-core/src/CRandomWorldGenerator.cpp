//
// Created by monty on 23-03-2017.
//
#include <map>
#include <functional>
#include <memory>
#include <array>
#include <sstream>
#include <vector>
#include <random>
#include "Vec2i.h"
#include "IMapElement.h"
#include "CTeam.h"
#include "CItem.h"
#include "CActor.h"
#include "CGameDelegate.h"
#include "CMap.h"
#include "CRandomWorldGenerator.h"

namespace Knights {
    std::string initAsBlankMap(char filler) {
        std::stringstream ss;

        for (int y = 0; y < kMapSize; ++y) {
            for (int x = 0; x < kMapSize; ++x) {
                ss << filler;
            }
            ss << '\n';
        }

        return ss.str();
    }

    void put(std::string &level, int x, int y, char element) {
        level[((kMapSize + 1) * y) + x] = element;
    }

    char get(std::string &level, int x, int y ) {
        return level[((kMapSize + 1) * y) + x];
    }

    int aliveNeighbours( std::string &level, int px, int py ) {
        int toReturn = 0;
        for ( int y = -1; y <= 1; ++y ) {
            if ( y + py < 0 || y + py >= kMapSize ) {
                continue;
            }

            for ( int x = -1; x <= 1; ++x ) {
                if ( x + px < 0 || x + px >= kMapSize  || ( x == 0 && y == 0 )) {
                    continue;
                }

                if ( get( level, x + px, y + py ) == '0' ) {
                    toReturn++;
                }
            }
        }

        return toReturn;
    }

    std::string germinate( std::string &level ) {
        std::string newGeneration = level;

        for ( int y = 0; y < kMapSize; ++y ) {
            for ( int x = 0; x < kMapSize; ++x ) {
                auto element = get( level, x, y);
                auto neighbours = aliveNeighbours( level, x, y );

                if ( element == '0' ) {
                    if ( neighbours < 2 ) {
                        put( newGeneration, x , y, 'O');
                    } else if ( neighbours == 2 || neighbours == 3 ) {
                        put( newGeneration, x , y, '0');
                    } else {
                        put( newGeneration, x , y, 'O');
                    }
                } else {
                    if ( neighbours == 3 ) {
                        put(newGeneration, x, y, '0');
                    }
                }
            }
        }

        return newGeneration;
    }

    void makeBuilding(std::string &level, int x0, int x1, int y0, int y1, char wall, char interior) {
        for (int y = y0; y <= y1; ++y) {
            for (int x = x0; x <= x1; ++x) {

                char choice = interior;

                if (x == x0 || x == x1 || y == y0 || y == y1) {
                    choice = wall;
                }

                put(level, x, y, choice);
            }
        }
    }

    void plantRandomSeed( std::string &level ) {

        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dis(0.0f, 1.0f);

        for ( int y = 0; y < kMapSize; ++y ) {
            for ( int x = 0; x < kMapSize; ++x ) {
                float number = dis( gen );
                put( level, x, y, (number > 0.5f ) ? '0' : 'O' );
            }
        }
    }

    void connectWithCorridors(std::string &level, Vec2i p0, Vec2i p1, char interior, char i0, char i1) {

        int x0 = std::min(p0.x, p1.x);
        int x1 = std::max(p0.x, p1.x);
        int y0 = std::min(p0.y, p1.y);
        int y1 = std::max(p0.y, p1.y);


        for (int x = x0; x <= x1; ++x) {
//            put( level, x, y0, interior );
            put(level, x, y1, interior);
        }

        for (int y = y0; y <= y1; ++y) {
            put(level, x0, y, interior);
//            put( level, x1, y, interior );
        }

        put(level, p0.x, p0.y, i0);
        put(level, p1.x, p1.y, i1);
    }

    std::string generate(int seed) {
        auto toReturn = initAsBlankMap('O');
        plantRandomSeed( toReturn );

//        put(toReturn, 10, kMapSize - 11, '0');
//        put(toReturn, 11, kMapSize - 11, '0');
//        put(toReturn, 12, kMapSize - 11, '0');
//        put(toReturn, 12, kMapSize - 12, '0');
//        put(toReturn, 11, kMapSize - 13, '0');
//
        for ( int generation = 0; generation < seed; ++generation ) {
            toReturn = germinate( toReturn );
        }

//        makeBuilding(toReturn, 20, 30, 14, 20, '1', '=');
//        makeBuilding(toReturn, 1, 10, 1, 9, '1', '=');
//        makeBuilding(toReturn, 33, 38, 34, 39, '1', '=');

//        connectWithCorridors(toReturn, {20, 18}, {10, 8}, '=', '~', '~');
//        connectWithCorridors(toReturn, {33, 37}, {25, 20}, '=', '~', '~');





        put(toReturn, 25, 17, '4');
//        put(toReturn, 24, 17, 'y');
//        put(toReturn, 24, 18, 'u');
//        put(toReturn, 6, 2, '5');
//        put(toReturn, 6, 3, '5');
//        put(toReturn, 7, 3, '5');

        return toReturn;
    }
}
