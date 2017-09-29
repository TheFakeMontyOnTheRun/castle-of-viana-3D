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
        fixed_point<int32_t , -16> angle{-45};
        fixed_point<int32_t , -16> increment = fixed_point<int32_t , -16>{ 9.0f / 32.0f };

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

    void CRenderer::draw( std::shared_ptr<odb::NativeBitmap> bitmap, int x0, int y0, int w, int h, int zValue ) {
//        fixed_point<int32_t, -16> stepX = fixed_point<int32_t, -16>{bitmap->getWidth() / static_cast<float>(w)};
//        fixed_point<int32_t, -16> stepY = fixed_point<int32_t, -16>{bitmap->getHeight() / static_cast<float>(h)};
//        int *pixelData = bitmap->getPixelData();
//        int bWidth = bitmap->getWidth();
//        fixed_point<int32_t, -16> px{0};
//        fixed_point<int32_t, -16> py{0};
//
//        int fillDX = std::max( 1, static_cast<int>(stepX) );
//        int fillDY = std::max( 1, static_cast<int>(stepY) );
//        int lastTexel = -1;
//        std::array< uint8_t, 4 > texel;
//
//        for ( int y = y0; y < ( y0 + h ); ++y ) {
//            px = 0;
//
//            for ( int x = x0; x < ( x0 + w ); ++x ) {
//
//
//                int pixel = pixelData[ ( bWidth * static_cast<int>( py )  ) + static_cast<int>(px) ];
//
//                if ( pixel != lastTexel ) {
//                    texel = std::array<uint8_t, 4>{0,
//                                                   static_cast<unsigned char>((pixel & 0x000000FF) >> 0),
//                                                   static_cast<unsigned char>((pixel & 0x0000FF00) >> 8),
//                                                   static_cast<unsigned char>((pixel & 0x00FF0000) >> 16)};
//                }
//                lastTexel = pixel;
//
//                if ( ( ( pixel & 0xFF000000 ) >> 24 ) > 0 ) {
//                    fill( x, y, fillDX, fillDY, texel);
//                }
//                px += stepX;
//            }
//            py += stepY;
//        }
    }

    void CRenderer::render(long ms) {

        if ( mNeedsToRedraw ) {
            mNeedsToRedraw = false;
            const static int halfYRes = yRes / 2;
            const static Knights::Vec2i mapSize = {40, 40};

            fixed_point<int32_t , -16> angle{-45};
            fixed_point<int32_t , -16> fov{ 90 };
            fixed_point<int32_t , -16> width{ xRes };
            fixed_point<int32_t , -16> increment = sg14::divide( fov, width );
            int textureWidth;
            int textureHeight;
            int *textureData;
            int columnHeight;
            auto lastDistance = fixed_point<int32_t , -16>{-1};
            int lastElement = -1;
            int dx = -1;
            int dy = -1;
            int dz = -1;
            int ux = -1;
            int uz = -1;
            int lastPixel = -1;
            std::array<uint8_t ,4> colour;
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
                    columnHeight = yRes / rayCollision.mSquaredDistance;
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

                fill(d, 0,        1, baseHeight,     {0, 96, 96, 96});
                fill(d, yRes - (baseHeight), 1, baseHeight, {0, 192, 192, 192});

                for (int y = 0; y <= dy; ++y) {

                    int v = ((textureHeight * y) / columnHeight);

                    while ( v >= textureHeight ) {
                        v -= textureHeight - 1;
                    }

                    int pixel = textureData[( textureWidth * v) + pixelColumn];

                    if ( pixel != lastPixel ) {
                        int r = static_cast<unsigned char>((pixel & 0xFF) >> 0);
                        int g = static_cast<unsigned char>((pixel & 0x00FF00) >> 8);
                        int b = static_cast<unsigned char>((pixel & 0xFF0000) >> 16);
                        colour[ 0 ] = 0;
                        colour[ 1 ] = r;
                        colour[ 2 ] = g;
                        colour[ 3 ] = b;
                    }
                    lastPixel = pixel;

//                if ( pixelColumn != ux && pixelColumn != uz ) {
//                    colour[ 1 ] = colour[ 1 ] >> 1;
//                    colour[ 2 ] = colour[ 2 ] >> 1;
//                    colour[ 3 ] = colour[ 3 ] >> 1;
//                    lastPixel = -1;
//                }

                    put( d, (baseHeight + y), colour );
                }

//
//            for (int y = baseHeight; y >=0; --y) {
//
//                colour[ 0 ] = 0;
//                colour[ 1 ] = static_cast<int>((255.0f * y)/baseHeight);
//                colour[ 2 ] = 0;
//                colour[ 3 ] = 0;
//
//                fill( d, (yRes - (baseHeight) + y), 1, 1, colour );
//            }

//            for (int z = 2; z < rayCollision.mSquaredDistance; ++z ) {
//
//                colour[ 0 ] = 0;
//                colour[ 1 ] = 255;//static_cast<int>((255.0f * z)/40);
//                colour[ 2 ] = 0;
//                colour[ 3 ] = 0;
//
//
//                fill( d, (halfYRes + (yRes / z) ), 1, 1, colour );
//
//                colour[ 0 ] = 0;
//                colour[ 1 ] = 128;//static_cast<int>((255.0f * z)/40);
//                colour[ 2 ] = 0;
//                colour[ 3 ] = 0;
//
//                fill( d, (halfYRes + (yRes / (z - 1)) ), 1, 1, colour );
//
//
////
////                colour[ 0 ] = 0;
////                colour[ 1 ] = 0;
////                colour[ 2 ] = 0;
////                colour[ 3 ] = 0;
////
////                baseHeight = ( (yRes / z) );
////                dy = (yRes / (z - 1)) - baseHeight;
////                baseHeight += halfYRes;
////
////                for (int y = 0; y <= dy; ++y) {
////
////                    int v = ((textureHeight * y) / dy);
////
////                    if ( v >= textureHeight ) {
////                        v -= textureHeight;
////                    }
////
////                    int pixel = textureData[( textureWidth * v) + pixelColumn];
////
////                    if ( pixel != lastPixel ) {
////                        int r = static_cast<unsigned char>((pixel & 0xFF) >> 0);
////                        int g = static_cast<unsigned char>((pixel & 0x00FF00) >> 8);
////                        int b = static_cast<unsigned char>((pixel & 0xFF0000) >> 16);
////                        colour[ 0 ] = 0;
////                        colour[ 1 ] = r;
////                        colour[ 2 ] = g;
////                        colour[ 3 ] = b;
////                    }
////                    lastPixel = pixel;
////
////                    fill( d, (baseHeight + y), 1, 1, colour );
////                }
//            }


                colour[ 0 ] = 0;
                colour[ 1 ] = 0;
                colour[ 2 ] = 0;
                colour[ 3 ] = 0;

            }

            int index = 0;
            lastDistance = -1;
//        for ( const auto& actor : mActorsRendered ) {
//
//            if ( mCachedDistances[index] != lastDistance ) {
//                columnHeight = (yRes / mCachedDistances[index]);
//                baseHeight = (halfYRes - (columnHeight) );
//            }
//            lastDistance = mCachedDistances[index];
//
//            draw( textures[ 4  ],
//                  mCachedAngle[ index ],
//                  baseHeight,
//                  2 * columnHeight,
//                  2 * columnHeight,
//                  mCachedDistances[index]);
//            ++index;
//        }

            mCachedAngle.clear();
            mActorsRendered.clear();
//        mCachedDistances.clear();

        }


        flip();
    }


    RayCollision CRenderer::castRay(int d, int offset, const array<array< int, 40>, 40> &map) {
        const static Knights::Vec2i blockSize = {fixed_point<int32_t, -16>(32), fixed_point<int32_t, -16>(32)};
        fixed_point<int32_t, -16> rx0 = fixed_point<int32_t , -16>{mCamera.x};
        fixed_point<int32_t, -16> ry0 = fixed_point<int32_t , -16>{mCamera.y};

        fixed_point<int32_t, -16> rx = rx0;
        fixed_point<int32_t, -16> ry = ry0;

        RayCollision collision;
        int angle = wrap360(360 - (mAngle + offset));

        auto sin_a = sines[angle];
        auto cos_a = cossines[angle];
        auto distance = fixed_point<int32_t, -16>{0};
        auto one = fixed_point<int32_t, -16>{1};
        int intX = static_cast<int>(rx);
        int intY = static_cast<int>(ry);
        auto bigger = std::max( cos_a, sin_a );

        auto increment = one;
        while (!map[intY][intX]) {
            rx -= sin_a;
            ry -= cos_a;
            distance += increment;
            intX = std::min( 39, std::max( 0, static_cast<int>(rx) ));
            intY = std::min( 39, std::max( 0, static_cast<int>(ry) ));
//            auto actor = mActors[ intY ][ intX ];
//            if ( actor ) {
//                mCachedDistances.push_back(sg14::multiply( distance, cossines[ wrap360( offset ) ] ));
//                mActorsRendered.insert(actor);
//                mCachedAngle.push_back(d);
//            }
        }

        bigger = cossines[ wrap360( offset ) ] / ( one + one );
        collision.mSquaredDistance =  static_cast<float>( multiply( distance, bigger ));

        auto integralX = fixed_point<int32_t , -16>{intX};
        auto integralY = fixed_point<int32_t , -16>{intY};

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
}
