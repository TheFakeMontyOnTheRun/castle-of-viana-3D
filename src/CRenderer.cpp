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
    };

    void CRenderer::drawMap(Knights::CMap &map, std::shared_ptr<Knights::CActor> current) {

        mCamera = current->getPosition();
        mAngle = (static_cast<int>(current->getDirection()) * 90);
        fixed_point<int32_t , -16> angle{-45};
        fixed_point<int32_t , -16> fov{ 90 };
        fixed_point<int32_t , -16> width{ 320 };
        fixed_point<int32_t , -16> increment = fixed_point<int32_t , -16>{ 90.0f / 320.0f };
        for (int d = 0; d < 320; ++d) {
            angle += increment;
            mCurrentScan[d] = castRay(static_cast<int>(angle), map);
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
                        for (int fillX = x; fillX < (x + fillDX); ++fillX) {

                            if ( fillX >= 320 || fillX < 0 ) {
                                continue;
                            }

                            if ( fillY >= 200 || fillY < 0 ) {
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


        const static Knights::Vec2i mapSize = {40, 40};

        fill(0, 0, xRes, yRes, {0, 96, 96, 96});
        fill(0, yRes / 2, xRes, yRes / 2, {0, 192, 192, 192});


        fixed_point<int32_t , -16> angle{-45};
        fixed_point<int32_t , -16> fov{ 90 };
        fixed_point<int32_t , -16> width{ 320 };
        fixed_point<int32_t , -16> increment = sg14::divide( fov, width );

        for (int d = 0; d < 320; ++d) {
            int index = 1;
            angle += increment;
            auto rayCollision = mCurrentScan[d];
            auto texture = textures[rayCollision.mElement];
            const int textureWidth = texture->getWidth();
            const int textureHeight = texture->getHeight();

            const int *textureData = texture->getPixelData();

            int distance = std::floor<int>(yRes * Q_rsqrt(rayCollision.mSquaredDistance));

            auto dx = rayCollision.mCollisionPoint.x;
            auto dz = rayCollision.mCollisionPoint.y;

            int columnHeight = distance;

            for (int y = 0; y <= columnHeight * (rayCollision.mHeight + 1); ++y) {

                int v = ((textureHeight * y) / columnHeight) % textureHeight;
                int ux = (textureWidth * dx) / mapSize.x;
                int uz = (textureWidth * dz) / mapSize.y;

                unsigned int pixel = textureData[(textureWidth * v) + ((ux + uz) % textureWidth)];

                int zColumn = d;

                if (zColumn > 0 && zColumn < 320) {
                    zBuffer[zColumn] = distance;
                }

                fill(d,
                     (yRes / 2 - (columnHeight * rayCollision.mHeight) + y),
                     1,
                     1,
                     {0,
                      static_cast<unsigned char>((pixel & 0xFF) >> 0),
                      static_cast<unsigned char>((pixel & 0x00FF00) >> 8),
                      static_cast<unsigned char>((pixel & 0xFF0000) >> 16)
                     }
                );
            }
        }
        flip();
    }


    RayCollision CRenderer::castRay(uint32_t offset, const Knights::CMap &map) {
        const static Knights::Vec2i blockSize = {fixed_point<int32_t, -16>(32), fixed_point<int32_t, -16>(32)};
        fixed_point<int32_t, -16> rx0 = fixed_point<int32_t , -16>{mCamera.x};
        fixed_point<int32_t, -16> ry0 = fixed_point<int32_t , -16>{mCamera.y};

        fixed_point<int32_t, -16> rx = rx0;
        fixed_point<int32_t, -16> ry = ry0;

        RayCollision collision;
        int angle = 360 - wrap360(mAngle + offset);

        fixed_point<int32_t, -16> sin_a = sines[angle];
        fixed_point<int32_t, -16> cos_a = cossines[angle];

        auto distance = 0;

        while (!map.isBlockViewAt(Knights::Vec2i{static_cast<int>(rx), static_cast<int>(ry)})) {
            rx -= sin_a;
            ry -= cos_a;
            distance += 1;
        }

        collision.mSquaredDistance = distance;
        int intX = static_cast<int>(rx);
        int intY = static_cast<int>(ry);
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
