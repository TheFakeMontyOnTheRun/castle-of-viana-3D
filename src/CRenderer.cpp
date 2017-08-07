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
#include "NativeBitmap.h"
#include "LoadPNG.h"

namespace odb {
    std::vector<std::shared_ptr<odb::NativeBitmap>> textures {
            loadPNG("res/tile0.png"),
            loadPNG("res/tile0.png"),
            loadPNG("res/tile1.png"),
            loadPNG("res/bricks0.png"),
            loadPNG("res/hero0.png"),
            loadPNG("res/fireball.png"),
    };

    void CRenderer::drawMap( Knights::CMap& map, std::shared_ptr<Knights::CActor> current ) {

        mCamera = current->getPosition();
        mAngle = (static_cast<int>(current->getDirection()) * 90);
        for (int d = -45; d < 45; ++d) {
            mCurrentScan[ d + 45 ] = castRay(d, map);
        }
//        std::cout << "--------" << std::endl;
//        for ( int y = 0; y < 40; ++y ) {
//            for ( int x = 0; x < 40; ++x ) {
//                std::cout << map.isBlockMovementAt({x, y });
//            }
//            std::cout << std::endl;
//        }
    }


    void CRenderer::draw( std::shared_ptr<odb::NativeBitmap> bitmap, int x0, int y0, int w, int h, int zValue ) {
        float stepX = static_cast<float>(bitmap->getWidth()) / static_cast<float>(w);
        float stepY = static_cast<float>(bitmap->getHeight()) / static_cast<float>(h);
        int *pixelData = bitmap->getPixelData();
        int bWidth = bitmap->getWidth();
        float px = 0;
        float py = 0;

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

    void CRenderer::render( long ms ) {

      //TODO: move into  constant
      const static Vec2f blockSize = { 32, 32 };

      //TODO: move into  constant
      const static Vec2f mapSize = { 40, 40 };

      fill( 0, 0, xRes, yRes, {0, 96, 96, 96} );
      fill( 0, yRes / 2, xRes, yRes / 2, {0, 192, 192, 192} );


      constexpr auto columnsPerDegree = (xRes / 90) + 1;
      auto column = 0;

        auto texture = textures[ 1 ];

        const int textureWidth = texture->getWidth();
        const int textureHeight = texture->getHeight();


      for (int d = -45; d < 45; ++d) {
          int index = 1;
          auto rayCollision = mCurrentScan[ d + 45 ];

//          if ( rayCollision.mElement > 0 && rayCollision.mElement < textures.size()  ) {
//              index = rayCollision.mElement;
//          }


          const int* textureData = textures[ index ]->getPixelData();

          int distance = (yRes * Q_rsqrt(rayCollision.mSquaredDistance));
          float hueX =  rayCollision.mCollisionPoint.mX;
          float hueZ = rayCollision.mCollisionPoint.mY;

          int cellX = hueX;
          int cellZ = hueZ;

          int dx = ( hueX - cellX ) * blockSize.mX;
          int dz = ( hueZ - cellZ ) * blockSize.mY;

          int columnHeight = distance;

          for ( int y = 0; y <= columnHeight * ( rayCollision.mHeight + 1 ); ++y ) {

              int v = (( textureHeight * y) / columnHeight ) % textureHeight;
              int ux = (textureWidth * dx) / mapSize.mX;
              int uz = (textureWidth * dz) / mapSize.mY;

              unsigned int pixel = textureData[ ( textureWidth * v ) + ((ux + uz ) % textureWidth) ];

              for ( int c = 0; c < columnsPerDegree; ++c ) {

                  int zColumn = column + c;

                  if (zColumn > 0 && zColumn < 320 ) {
                      zBuffer[ zColumn ] = distance;
                  }
              }

              fill( column,
                    (yRes / 2 - (columnHeight * rayCollision.mHeight) + y ),
                    (columnsPerDegree),
                    1,
                    {0 ,
                     static_cast<unsigned char>((pixel & 0xFF) >> 0),
                     static_cast<unsigned char>((pixel & 0x00FF00) >> 8),
                     static_cast<unsigned char>((pixel & 0xFF0000) >> 16)
                    }
              );
          }

          column += columnsPerDegree;
      }

        column = -columnsPerDegree;
//        for (int d = -45; d < 45; ++d) {
//
//            column += columnsPerDegree;
//
//            for ( const auto& c : mGameSnapshot.mActorAppearances ) {
//
//                float angle = c.mAngle;
//
//                if( angle != d ) {
//                    continue;
//                }
//
//                auto rayCollision = mGameSnapshot.mCurrentScan[d + 45];
//
//                if ( rayCollision.mSquaredDistance < c.mSquaredDistance ) {
//                    continue;
//                }
//
//                int distance =  (yRes * Q_rsqrt(c.mSquaredDistance));
//
//                int columnHeight = distance;
//
//                draw( c.mType == EActorType::kEnemy ? textures[ 4  ] : textures[ 5  ],
//                      column,
//                      (yRes / 2 - (columnHeight * rayCollision.mHeight) ) + ( c.mType == EActorType::kEnemy ? columnHeight / 2 : columnHeight) ,
//                      2 * columnHeight,
//                      2 * columnHeight, distance);
//            }
//        }




        flip();

  }


    RayCollision CRenderer::castRay(float offset, const Knights::CMap& map) {

        float rx0 = mCamera.x;
        float ry0 = mCamera.y;

        float rx = rx0;
        float ry = ry0;
        RayCollision collision;
        int angle = 360 - wrap360( mAngle + offset);

        auto sin_a = sines[ angle ];
        auto cos_a = cossines[ angle ];

        while ( !map.isBlockViewAt({ rx, ry} ) ) {
            rx -= sin_a;
            ry -= cos_a;
        }


        float dx = rx - rx0;
        float dy = ry - ry0;


        collision.mSquaredDistance = ((( dx * dx ) + ( dy * dy )) * cossines[ wrap360( offset)  ] * 16.0f );
        collision.mCollisionPoint = { rx, ry };
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
