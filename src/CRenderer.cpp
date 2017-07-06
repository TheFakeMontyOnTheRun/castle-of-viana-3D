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

    auto texture = loadPNG("res/tile0.png");

    void CRenderer::setSnapshot( const CGameSnapshot& snapshot ) {
        this->mGameSnapshot = snapshot;
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

        const int textureWidth = texture->getWidth();
        const int textureHeight = texture->getHeight();
        const int* textureData = texture->getPixelData();

      for (int d = -45; d < 45; ++d) {
          auto rayCollision = mGameSnapshot.mCurrentScan[ d + 45 ];
          float ray = rayCollision.mCachedDistance;
          int distance = (yRes / ray);
          float hueX =  rayCollision.mCollisionPoint.mX;
          float hueZ = rayCollision.mCollisionPoint.mY;

          int cellX = hueX;
          int cellZ = hueZ;

          int dx = ( hueX - cellX ) * blockSize.mX;
          int dz = ( hueZ - cellZ ) * blockSize.mY;

          int columnHeight = distance;

          for ( int y = 0; y <= columnHeight; ++y ) {

              int v = ( textureHeight * y) / columnHeight;
              int ux = (textureWidth * dx) / mapSize.mX;
              int uz = (textureWidth * dz) / mapSize.mY;

              unsigned int pixel = textureData[ ( textureWidth * v ) + ((ux + uz ) % textureWidth) ];

              fill( column,
                    (yRes / 2 - (distance / 2) + y ),
                    (columnsPerDegree),
                    1,
                    {0 , (pixel & 0xFF) >> 0, (pixel & 0x00FF00) >> 8, (pixel & 0xFF0000) >> 16 }
              );
          }

          column += columnsPerDegree;
      }



      flip();

  }
}
