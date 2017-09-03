#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <map>

#include <cstdlib>
#include <cstdio>
#include <functional>
#include <cmath>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <memory>
#include <iostream>
#include <map>
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <sg14/fixed_point>

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
    std::vector<std::shared_ptr<odb::NativeBitmap>> textures{
            loadPNG("res/tile0.png"),
            loadPNG("res/tile0.png"),
            loadPNG("res/bricks0.png"),
            loadPNG("res/hero0.png"),
    };

    void CRenderer::drawMap(Knights::CMap &map, std::shared_ptr<Knights::CActor> current) {

        mCamera = current->getPosition();
        mAngle = (static_cast<int>(current->getDirection()) * 90);
        fixed_point<int32_t , -16> angle{-45};
        fixed_point<int32_t , -16> increment = fixed_point<int32_t , -16>{ 9.0f / 32.0f };
        for (int d = 0; d < 320; ++d) {
            angle += increment;
            mCurrentScan[d] = castRay(d, static_cast<int>(angle), map);
        }
    }

    void CRenderer::draw( std::shared_ptr<odb::NativeBitmap> bitmap, int x0, int y0, int w, int h, int zValue ) {
        fixed_point<int32_t, -16> stepX = fixed_point<int32_t, -16>{bitmap->getWidth() / static_cast<float>(w)};
        fixed_point<int32_t, -16> stepY = fixed_point<int32_t, -16>{bitmap->getHeight() / static_cast<float>(h)};
        int *pixelData = bitmap->getPixelData();
        int bWidth = bitmap->getWidth();
        //fixed_point<int32_t, -16>
        fixed_point<int32_t, -16> px{0};
        fixed_point<int32_t, -16> py{0};

        int fillDX = std::max( 1, static_cast<int>(stepX) );
        int fillDY = std::max( 1, static_cast<int>(stepY) );

        for ( int y = y0; y < ( y0 + h ); ++y ) {
            px = 0;

            for ( int x = x0; x < ( x0 + w ); ++x ) {

                int pixel = pixelData[ ( bWidth * static_cast<int>( py )  ) + static_cast<int>(px) ];

                if ( ( ( pixel & 0xFF000000 ) >> 24 ) > 0 ) {

                    auto texel = std::array< uint8_t, 4 >{0 ,
                                                          static_cast<unsigned char>((pixel & 0xFF) >> 0),
                                                          static_cast<unsigned char>((pixel & 0x00FF00) >> 8),
                                                          static_cast<unsigned char>((pixel & 0xFF0000) >> 16) };

                    for ( int fillY = y; fillY < (y + fillDY); ++fillY  ) {

                        if ( fillY >= 200 || fillY < 0 ) {
                            continue;
                        }

                        for (int fillX = x; fillX < (x + fillDX); ++fillX) {

                            if ( fillX >= 320 || fillX < 0 ) {
                                continue;
                            }

                            if ( zBuffer[ fillX ] <= zValue ) {
                                fill( fillX, fillY, 1,1, texel);
                                zBuffer[ fillX ] = zValue;
                            }
                        }
                    }
                }
                px += stepX;
            }
            py += stepY;
        }
    }

    void CRenderer::render(long ms) {

        const static int halfYRes = yRes / 2;
        const static Knights::Vec2i mapSize = {40, 40};

        fill(0, 0, xRes, yRes, {0, 96, 96, 96});
        fill(0, halfYRes, xRes, halfYRes, {0, 192, 192, 192});

        fixed_point<int32_t , -16> angle{-45};
        fixed_point<int32_t , -16> fov{ 90 };
        fixed_point<int32_t , -16> width{ 320 };
        fixed_point<int32_t , -16> increment = sg14::divide( fov, width );
        int textureWidth;
        int textureHeight;
        int *textureData;
        int columnHeight;
        int lastDistance = -1;
        int lastElement = -1;
        int dx = -1;
        int dz = -1;
        int ux = -1;
        int uz = -1;
        int lastPixel = -1;
        int r = -1;
        int g = -1;
        int b = -1;
        int baseHeight = -1;
        int lastHeight = -1;
        Knights::Vec2i collisionPoint{ -1, -1};

        for (int d = 0; d < 320; ++d) {
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

            if ( collisionPoint.x != rayCollision.mCollisionPoint.x || collisionPoint.y != rayCollision.mCollisionPoint.y ) {
                dx = rayCollision.mCollisionPoint.x;
                dz = rayCollision.mCollisionPoint.y;
                ux = (textureWidth * dx) / mapSize.x;
                uz = (textureWidth * dz) / mapSize.y;
            }
            collisionPoint.x = rayCollision.mCollisionPoint.x;
            collisionPoint.y = rayCollision.mCollisionPoint.y;

            if ( baseHeight != lastHeight ) {
                baseHeight = halfYRes - (columnHeight * rayCollision.mHeight);
            }
            lastHeight = columnHeight;

            if (d > 0 && d < 320) {
                zBuffer[d] = columnHeight;
            }

            int pixelColumn = ((ux + uz) % textureWidth);

            for (int y = 0; y <= columnHeight * (rayCollision.mHeight + 1); ++y) {

                int v = ((textureHeight * y) / columnHeight) % textureHeight;
                unsigned int pixel = textureData[(textureWidth * v) + pixelColumn];

                if ( pixel != lastPixel ) {
                    r = static_cast<unsigned char>((pixel & 0xFF) >> 0);
                    g = static_cast<unsigned char>((pixel & 0x00FF00) >> 8);
                    b = static_cast<unsigned char>((pixel & 0xFF0000) >> 16);
                }
                lastPixel = pixel;

                fill( d, (baseHeight + y), 1, 1, {0, r, g, b} );
            }
        }

        int index = 0;
        lastDistance = -1;
        for ( const auto& actor : mActorsRendered ) {

            if ( mCachedDistances[index] != lastDistance ) {
                columnHeight = (yRes / mCachedDistances[index]);
                baseHeight = (halfYRes - (columnHeight) );
            }
            lastDistance = mCachedDistances[index];

            draw( textures[ 3  ],
                  mCachedAngle[ index ],
                  baseHeight,
                  2 * columnHeight,
                  2 * columnHeight,
                  mCachedDistances[index]);
            ++index;
        }

        mCachedAngle.clear();
        mActorsRendered.clear();
        mCachedDistances.clear();

        flip();
    }


    RayCollision CRenderer::castRay(int d, int offset, Knights::CMap &map) {
        const static Knights::Vec2i blockSize = {fixed_point<int32_t, -16>(32), fixed_point<int32_t, -16>(32)};
        fixed_point<int32_t, -16> rx0 = fixed_point<int32_t , -16>{mCamera.x};
        fixed_point<int32_t, -16> ry0 = fixed_point<int32_t , -16>{mCamera.y};

        fixed_point<int32_t, -16> rx = rx0;
        fixed_point<int32_t, -16> ry = ry0;

        RayCollision collision;
        int angle = wrap360(360 - (mAngle + offset));

        fixed_point<int32_t, -16> sin_a = sines[angle];
        fixed_point<int32_t, -16> cos_a = cossines[angle];

        auto distance = fixed_point<int32_t, -16>{0};
        auto one = fixed_point<int32_t, -16>{1};
        int intX = static_cast<int>(rx);
        int intY = static_cast<int>(ry);

        while (!map.isBlockViewAt(Knights::Vec2i{intX, intY})) {
            rx -= sin_a;
            ry -= cos_a;
            distance += one;
            intX = static_cast<int>(rx);
            intY = static_cast<int>(ry);
            auto actor = map.getActorAt({intX, intY });
            if ( actor != nullptr && actor != map.getAvatar() ) {
                mCachedDistances.push_back(static_cast<float>( multiply( distance, cossines[ wrap360( offset ) ] )));
                mActorsRendered.insert(actor);
                mCachedAngle.push_back(d);
            }
        }

        collision.mSquaredDistance = static_cast<float>( multiply( distance, cossines[ wrap360( offset ) ] ));

        auto integralX = fixed_point<int32_t , -16>{intX};
        auto integralY = fixed_point<int32_t , -16>{intY};

        collision.mCollisionPoint = Knights::Vec2i{
                static_cast<int>(multiply( blockSize.x, (rx - integralX ) )),
                static_cast<int>(multiply( blockSize.y, (ry - integralY ) ))
        };

        auto element = map.getElementAt({intX, intY});

        switch( element ) {
            case '1':
                collision.mElement =  1;
                collision.mHeight =  1;
                break;
            case 'I':
                collision.mElement =  2;
                collision.mHeight =  2;
                break;
            default:
                collision.mHeight = 0;
                collision.mElement =  0;
        }

        return collision;
    }

    Knights::CommandType CRenderer::getInput() {
        auto toReturn = mBufferedCommand;
        mBufferedCommand = '.';
        return toReturn;
    }
}
