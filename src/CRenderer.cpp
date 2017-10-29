#include <string>
#include <iostream>
#include <memory>
#include <utility>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <functional>
#include <cmath>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <memory>
#include <iostream>
#include <map>
#include <iostream>
#include <string>
#include <sg14/fixed_point>
#include <EASTL/vector.h>
#include <EASTL/array.h>

using eastl::vector;
using eastl::array;

using sg14::fixed_point;

#include "NativeBitmap.h"
#include "Vec2i.h"
#include "IMapElement.h"
#include "CTeam.h"
#include "CItem.h"
#include "CActor.h"
#include "CGameDelegate.h"
#include "CMap.h"
#include "IRenderer.h"
#include "RaycastCommon.h"
#include "CRenderer.h"
#include "LoadPNG.h"

namespace odb {
    vector<std::shared_ptr<odb::NativeBitmap>> textures{
            loadPNG("res/tile0.png"),
            loadPNG("res/tile0.png"),
            loadPNG("res/bricks0.png"),
            loadPNG("res/tile1.png"),
            loadPNG("res/hero0.png"),
    };

    void CRenderer::drawMap(Knights::CMap &map, std::shared_ptr<Knights::CActor> current) {

        mCamera = current->getPosition();
        mAngle = (static_cast<int>(current->getDirection()) * 90);
        FixP angle{-45};
        FixP increment = FixP{ 9 } / FixP{ 32 };
        mCameraActor = current;

        if (!mCached ) {
            mCached = true;
            mNeedsToRedraw = true;

            for ( int y = 0; y < 40; ++y ) {
                for ( int x = 0; x < 40; ++x ) {

                    auto actor = map.getActorAt({x, y });

                    if ( actor != nullptr && actor != map.getAvatar() ) {
                        mActors[ y ][ x ] = actor->getId();
                    } else  {
                        mActors[ y ][ x ] = 0;
                    }

                    auto element = map.getElementAt({x, y});

                    switch( element ) {
                        case '1':
                            mCache[ y ][ x ] = 1;
                            break;
                        case 'I':
                            mCache[ y ][ x ] = 2;
                            break;
                        default:
                            mCache[ y ][ x ] = 0;
                    }
                }
            }

            for (int d = 0; d < xRes; ++d) {
                angle += increment;
                mCurrentScan[d] = castRay(d, static_cast<int>(angle), mCache);
            }
        }
    }


    void CRenderer::draw( std::shared_ptr<odb::NativeBitmap> bitmap, int x0, int y0, int w, int h, FixP zValue ) {
        auto integerDistance = static_cast<int>(zValue);

        if ( integerDistance > 40 || integerDistance < 1) {
            return;
        }

        FixP stepX = ( FixP{ bitmap->getWidth()  } / FixP{ w } );
        FixP stepY = ( FixP{ bitmap->getHeight() } / FixP{ h } );
        FixP px{0};
        FixP py{0};

        int *pixelData = bitmap->getPixelData();
        int bWidth = bitmap->getWidth();
        int fillDX = std::max( 1, static_cast<int>(stepX) );
        int fillDY = std::max( 1, static_cast<int>(stepY) );
//        int lastTexel = -1;
//
//        array< uint8_t, 4 > texel;

        for ( int y = y0; y < ( y0 + h ); ++y ) {
            px = 0;

            for ( int x = x0; x < ( x0 + w ); ++x ) {


                int pixel = pixelData[ ( bWidth * static_cast<int>( py )  ) + static_cast<int>(px) ];

//                if ( pixel != lastTexel ) {
//                    texel = array<uint8_t, 4>{0,
//                                                   static_cast<unsigned char>((pixel & 0x000000FF)      ),
//                                                   static_cast<unsigned char>((pixel & 0x0000FF00) >> 8 ),
//                                                   static_cast<unsigned char>((pixel & 0x00FF0000) >> 16)};
//                }
//                lastTexel = pixel;

                if ( ( ( pixel & 0xFF000000 ) > 0 ) &&
                        ( x > -fillDX ) &&
                        ( y > -fillDY ) &&
                        ( x < 320 + fillDX ) &&
                        ( y < 128 + fillDY )
                        ) {
//                    fill( x, y, fillDX, fillDY, texel);

                    put( x, y, pixel);
                }
                px += stepX;
            }
            py += stepY;
        }
    }

    void CRenderer::render(long ms) {

        if ( mNeedsToRedraw ) {
            mNeedsToRedraw = false;
            const static int halfYRes = yRes / 2;
            const static Knights::Vec2i mapSize = {40, 40};

            FixP angle{-45};
            FixP fov{ 90 };
            FixP width{ xRes };
            FixP increment = fov / width;
            int textureWidth;
            int textureHeight;
            int *textureData;
            int columnHeight;
            auto lastDistance = FixP{-1};
            int lastElement = -1;
            int dx = -1;
            int dy = -1;
            int dz = -1;
            int ux = -1;
            int uz = -1;
            int lastPixel = -1;
//            array<uint8_t ,4> colour;
            int baseHeight = -1;
            int lastHeight = -1;
            int pixelColumn = -1;
            Knights::Vec2i collisionPoint{ -1, -1};

//        drawCeiling();
            // drawFloor();

            for (int d = 0; d < xRes; ++d) {
                angle += increment;
                auto rayCollision = mCurrentScan[d];

                if (rayCollision.mElement != lastElement ) {
                    std::shared_ptr<odb::NativeBitmap> texture = textures[rayCollision.mElement];
                    textureWidth = texture->getWidth();
                    textureHeight = texture->getHeight();
                    textureData = texture->getPixelData();
                }
                lastElement = rayCollision.mElement;

                if ( lastDistance != rayCollision.mSquaredDistance ) {
                    columnHeight = static_cast<int>(divide( fixedYRes, rayCollision.mSquaredDistance ));
                }
                lastDistance = rayCollision.mSquaredDistance;

                if ( collisionPoint.x != rayCollision.mCollisionPoint.x ||
                     collisionPoint.y != rayCollision.mCollisionPoint.y ) {
                    dx = rayCollision.mCollisionPoint.x;
                    dz = rayCollision.mCollisionPoint.y;
                    ux = (textureWidth * dx) / mapSize.x;
                    uz = (textureWidth * dz) / mapSize.y;
                    pixelColumn = (ux + uz);

                    if ( pixelColumn >= textureWidth ) {
                        pixelColumn -= textureWidth;
                    }
                }
                collisionPoint.x = rayCollision.mCollisionPoint.x;
                collisionPoint.y = rayCollision.mCollisionPoint.y;

                if ( baseHeight != lastHeight ) {
                    baseHeight = halfYRes - (columnHeight * (rayCollision.mHeight));
                    dy = (columnHeight * (rayCollision.mHeight + 1));
                }
                lastHeight = columnHeight;

                if (d >= 0 && d < xRes) {
                    zBuffer[d] = columnHeight;
                }

                if ( baseHeight >= 0 ) {
                    fill( 32 + d, 0,        1, baseHeight,     {0, 96, 96, 96});
                    fill( 32 + d, yRes - (baseHeight), 1, baseHeight, {0, 192, 192, 192});
                }

                for (int y = 0; y <= dy; ++y) {

                    int v = ((textureHeight * y) / columnHeight);

                    while ( v >= textureHeight ) {
                        v -= textureHeight - 1;
                    }

                    int pixel = textureData[( textureWidth * v) + pixelColumn];

//                    if ( pixel != lastPixel ) {
//                        int r = static_cast<unsigned char>((pixel & 0xFF) >> 0);
//                        int g = static_cast<unsigned char>((pixel & 0x00FF00) >> 8);
//                        int b = static_cast<unsigned char>((pixel & 0xFF0000) >> 16);
//                        colour[ 0 ] = 0;
//                        colour[ 1 ] = r;
//                        colour[ 2 ] = g;
//                        colour[ 3 ] = b;
//                    }
//                    lastPixel = pixel;

//                    if ( uz == 0 ) {
//                        colour[ 1 ] = (colour[ 1 ] >> 1) + (colour[ 1 ] >> 2);
//                        colour[ 2 ] = (colour[ 2 ] >> 1) + (colour[ 2 ] >> 2);
//                        colour[ 3 ] = (colour[ 3 ] >> 1) + (colour[ 3 ] >> 2);
//                        lastPixel = -1;
//                    }


//                    if ( 0 == ux || ux >= 24 ) {
//                        colour[ 1 ] = (colour[ 1 ] >> 1) + (colour[ 1 ] >> 2) + (colour[ 1 ] >> 2);
//                        colour[ 2 ] = (colour[ 2 ] >> 1) + (colour[ 2 ] >> 2) + (colour[ 2 ] >> 2);
//                        colour[ 3 ] = (colour[ 3 ] >> 1) + (colour[ 3 ] >> 2) + (colour[ 3 ] >> 2);
//                        lastPixel = -1;
//                    }

                    int finalY = (baseHeight + y);

                    if ( finalY >= 0 && finalY < yRes ) {
                        put( 32 + d, finalY, pixel );
                    }
                }

//                colour[ 0 ] = 0;
//                colour[ 1 ] = 0;
//                colour[ 2 ] = 0;
//                colour[ 3 ] = 0;

            }

            int index = 0;
            lastDistance = -1;

            for ( const auto& actor : mActorsRendered ) {

                if ( mCachedDistances[index] != lastDistance ) {
                    columnHeight = (yRes / mCachedDistances[index]);
                    baseHeight = (halfYRes - (columnHeight) );
                }

                lastDistance = mCachedDistances[index];

                draw( textures[ 4  ],
                      32 + mCachedAngle[ index ],
                      baseHeight,
                      2 * columnHeight,
                      2 * columnHeight,
                      mCachedDistances[index]);
                ++index;
            }

            mCachedAngle.clear();
            mActorsRendered.clear();
            mCachedDistances.clear();

            fillSidebar();
            fillUnderbar();
        }

        flip();
    }


    RayCollision CRenderer::castRay(int d, int offset, const array<array< int, 40>, 40> &map) {
        const static Knights::Vec2i blockSize = {FixP(32), FixP(32)};
        FixP rx0 = FixP{mCamera.x};
        FixP ry0 = FixP{mCamera.y};

        FixP rx = rx0;
        FixP ry = ry0;

        RayCollision collision;
        int angle = wrap360(360 - (mAngle + offset));

        auto sin_a = sines[angle];
        auto cos_a = cossines[angle];
        auto distance = FixP{0};
        auto one = FixP{1};
        int intX = static_cast<int>(rx);
        int intY = static_cast<int>(ry);
        FixP bigger;

        auto increment = one;

        while (!map[intY][intX]) {
            rx -= sin_a;
            ry -= cos_a;
            distance += increment;
            intX = std::min( 39, std::max( 0, static_cast<int>(rx) ));
            intY = std::min( 39, std::max( 0, static_cast<int>(ry) ));
            auto actor = mActors[ intY ][ intX ];

            if ( actor && std::count( std::begin(mActorsRendered), std::end(mActorsRendered), actor ) == 0 ) {
                mCachedDistances.push_back(sg14::multiply( distance, cossines[ wrap360( offset ) ] ));
                mActorsRendered.push_back(actor);
                mCachedAngle.push_back(d);
            }
        }

        bigger = ( cossines[ wrap360( offset ) ] / (  one + one ) );
        collision.mSquaredDistance = ( multiply( distance, bigger ));

        auto integralX = FixP{intX};
        auto integralY = FixP{intY};

        collision.mCollisionPoint = Knights::Vec2i{
                static_cast<int>(multiply( blockSize.x, (rx - integralX ) )),
                static_cast<int>(multiply( blockSize.y, (ry - integralY ) ))
        };

        collision.mElement = mCache[intY][intX];
        collision.mHeight =  1; //mCache[intY][intX];

        return collision;
    }

    Knights::CommandType CRenderer::getInput() {
        auto toReturn = mBufferedCommand;
        mBufferedCommand = '.';
        return toReturn;
    }

    void CRenderer::fillSidebar() {
        bitblt( textures[ 2 ], 0, 0 * 32 );
        bitblt( textures[ 2 ], 0, 1 * 32 );
        bitblt( textures[ 2 ], 0, 2 * 32 );
        bitblt( textures[ 2 ], 0, 3 * 32 );

        bitblt( textures[ 2 ], 32 + 256, 0 * 32 );
        bitblt( textures[ 2 ], 32 + 256, 1 * 32 );
        bitblt( textures[ 2 ], 32 + 256, 2 * 32 );
        bitblt( textures[ 2 ], 32 + 256, 3 * 32 );
    }

    void CRenderer::fillUnderbar() {
        for ( int x = 0; x < 10; ++x ) {
            bitblt( textures[ 2 ], x * 32, 128 );
        }
    }
}
