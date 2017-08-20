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
    };

    void CRenderer::drawMap(Knights::CMap &map, std::shared_ptr<Knights::CActor> current) {

        mCamera = current->getPosition();
        mAngle = (static_cast<int>(current->getDirection()) * 90);
        for (int d = -45; d < 45; ++d) {
            mCurrentScan[d + 45] = castRay(d, map);
        }
    }

    void CRenderer::render(long ms) {


        const static Knights::Vec2i mapSize = {40, 40};

        fill(0, 0, xRes, yRes, {0, 96, 96, 96});
        fill(0, yRes / 2, xRes, yRes / 2, {0, 192, 192, 192});


        constexpr auto columnsPerDegree = (xRes / 90) + 1;
        auto column = 0;

        auto texture = textures[1];

        const int textureWidth = texture->getWidth();
        const int textureHeight = texture->getHeight();


        for (int d = -45; d < 45; ++d) {
            int index = 1;
            auto rayCollision = mCurrentScan[d + 45];

            const int *textureData = textures[index]->getPixelData();

            int distance = std::floor<int>(yRes * Q_rsqrt(rayCollision.mSquaredDistance));

            auto dx = rayCollision.mCollisionPoint.x;
            auto dz = rayCollision.mCollisionPoint.y;

            int columnHeight = distance;

            for (int y = 0; y <= columnHeight * (rayCollision.mHeight + 1); ++y) {

                int v = ((textureHeight * y) / columnHeight) % textureHeight;
                int ux = (textureWidth * dx) / mapSize.x;
                int uz = (textureWidth * dz) / mapSize.y;

                unsigned int pixel = textureData[(textureWidth * v) + ((ux + uz) % textureWidth)];

                for (int c = 0; c < columnsPerDegree; ++c) {

                    int zColumn = column + c;

                    if (zColumn > 0 && zColumn < 320) {
                        zBuffer[zColumn] = distance;
                    }
                }

                fill(column,
                     (yRes / 2 - (columnHeight * rayCollision.mHeight) + y),
                     (columnsPerDegree),
                     1,
                     {0,
                      static_cast<unsigned char>((pixel & 0xFF) >> 0),
                      static_cast<unsigned char>((pixel & 0x00FF00) >> 8),
                      static_cast<unsigned char>((pixel & 0xFF0000) >> 16)
                     }
                );
            }

            column += columnsPerDegree;
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
        int distance = 0;

        while (!map.isBlockViewAt(Knights::Vec2i{static_cast<int>(rx), static_cast<int>(ry)})) {
            rx -= sin_a;
            ry -= cos_a;
            distance++;
        }

        collision.mSquaredDistance = distance;
        auto integralX = fixed_point<int32_t , -16>{static_cast<int>(rx)};
        auto integralY = fixed_point<int32_t , -16>{static_cast<int>(ry)};
        collision.mCollisionPoint = Knights::Vec2i{
                static_cast<int>(multiply( blockSize.x, (rx - integralX ) )),
                static_cast<int>(multiply( blockSize.y, (ry - integralY ) ))
        };
        collision.mHeight = 1;
        collision.mElement = 1;
        return collision;
    }

    Knights::CommandType CRenderer::getInput() {
        auto toReturn = mBufferedCommand;
        mBufferedCommand = '.';
        return toReturn;
    }
}
