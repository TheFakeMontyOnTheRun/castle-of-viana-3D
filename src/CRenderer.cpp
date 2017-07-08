#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
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


#include "Common.h"
#include "CGameSnapshot.h"
#include "CGame.h"
#include "CRenderer.h"
#include "NativeBitmap.h"
#include "LoadPNG.h"

namespace odb {
    std::vector<std::shared_ptr<odb::NativeBitmap>> textures {
            loadPNG("res/tile0.png"),
            loadPNG("res/tile0.png"),
            loadPNG("res/tile1.png"),
            loadPNG("res/bricks0.png"),
    };

    void CRenderer::setSnapshot( const CGameSnapshot& snapshot ) {
        this->mGameSnapshot = snapshot;
    }


    void CRenderer::draw( std::shared_ptr<odb::NativeBitmap> bitmap, int x0, int y0, int w, int h ) {
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
                    fill( x, y, fillDX, fillDY,
                          {0 ,
                           static_cast<unsigned char>((pixel & 0xFF) >> 0),
                           static_cast<unsigned char>((pixel & 0x00FF00) >> 8),
                           static_cast<unsigned char>((pixel & 0xFF0000) >> 16)
                          }
                    );
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


      constexpr auto columnsPerDegree = (xRes / 90)+1;
      auto column = 0;

        auto texture = textures[ 1 ];

        const int textureWidth = texture->getWidth();
        const int textureHeight = texture->getHeight();


      for (int d = -45; d < 45; ++d) {
          int index = 1;
          auto rayCollision = mGameSnapshot.mCurrentScan[ d + 45 ];

          if ( rayCollision.mElement > 0 && rayCollision.mElement < textures.size()  ) {
              index = rayCollision.mElement;
          }


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



      flip();

  }
}
