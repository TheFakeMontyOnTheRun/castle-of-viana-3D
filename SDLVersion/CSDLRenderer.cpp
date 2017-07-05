#include <utility>
#include <functional>
#include <array>
#include <vector>
#include <memory>

#include "Common.h"
#include "CRenderer.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <cmath>
#include "NativeBitmap.h"
#include "LoadPNG.h"

namespace odb {
    const int xRes = 800;
    const int yRes = 600;

    auto texture = loadPNG("res/tile0.png");

    SDL_Surface *video;

    CRenderer::CRenderer( CControlCallback keyPressedCallback, CControlCallback keyReleasedCallback ):
    mOnKeyPressedCallback( keyPressedCallback ), mOnKeyReleasedCallback( keyReleasedCallback )
    {
        SDL_Init( SDL_INIT_EVERYTHING );
        video = SDL_SetVideoMode( xRes, yRes, 32, 0 );
    }

    void CRenderer::setSnapshot( const CGameSnapshot& snapshot ) {
        this->mGameSnapshot = snapshot;
    }

    void CRenderer::sleep( long ms ) {
        SDL_Delay(33);
    }

    void CRenderer::handleSystemEvents() {
        SDL_Event event;

        while ( SDL_PollEvent( &event ) ) {

            if( event.type == SDL_QUIT ) {
#ifndef __EMSCRIPTEN__
                exit(0);
#endif
            }

            if ( event.type == SDL_KEYUP ) {
                switch ( event.key.keysym.sym ) {
		case SDLK_q:
#ifndef __EMSCRIPTEN__
		  exit(0);
#endif
		case SDLK_LEFT:
                        mOnKeyReleasedCallback( ECommand::kLeft );
                        break;
		default:
		  break;
                }
            }

            if ( event.type == SDL_KEYDOWN ) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        mOnKeyPressedCallback(ECommand::kLeft);
                        break;
                    case SDLK_RIGHT:
                        mOnKeyPressedCallback(ECommand::kRight);
                        break;
                    case SDLK_UP:
                        mOnKeyPressedCallback(ECommand::kUp);
                        break;
                    case SDLK_DOWN:
                        mOnKeyPressedCallback(ECommand::kDown);
                        break;

                    default:
                        break;
                }
            }

        }
    }

    void CRenderer::fill( int x1, int y1, int w, int h, std::array<int,4> colour ) {
        SDL_Rect rect;
        rect.x = x1;
        rect.y = y1;
        rect.w = w;
        rect.h = h;

        SDL_FillRect(video, &rect, SDL_MapRGB(video->format, colour[ 1 ], colour[ 2 ], colour[ 3 ] ) );
    }

    void CRenderer::flip() {
        SDL_Flip(video);
    }

    void CRenderer::render( long ms ) {

        //TODO: move into  constant
        const static Vec2f blockSize = { 32, 32 };

        //TODO: move into  constant
        const static Vec2f mapSize = { 40, 40 };

        fill( 0, 0, xRes, yRes, {0, 96, 96, 96} );
        fill( 0, yRes / 2, xRes, yRes / 2, {0, 192, 192, 192} );


        constexpr auto columnsPerDegree = xRes / 90;
        auto column = 0;

        for (int d = -45; d < 45; ++d) {
            auto rayCollision = mGameSnapshot.mCurrentScan[ d + 45 ];
            float ray = rayCollision .mCachedDistance;
            int distance = (yRes / ray);
            float hueX =  rayCollision.mCollisionPoint.mX;
            float hueZ = rayCollision.mCollisionPoint.mY;

            int cellX = hueX;
            int cellZ = hueZ;

            int dx = ( hueX - cellX ) * blockSize.mX;
            int dz = ( hueZ - cellZ ) * blockSize.mY;

            int columnHeight = distance;

            for ( int y = 0; y < columnHeight; ++y ) {

                int v = ( texture->getHeight() * y) / columnHeight;
                int ux = (texture->getWidth() * dx) / mapSize.mX;
                int uz = (texture->getWidth() * dz) / mapSize.mY;
                int pixel = texture->getPixelData()[ ( texture->getWidth() * v ) + ((ux + uz ) % texture->getWidth()) ];

                fill( column,
                      (yRes / 2 - (distance / 2) + y ),
                      (columnsPerDegree),
                      1,
                      {0 , pixel & 0xFF000000 >> 8, pixel & 0x00FF0000 >> 4, pixel & 0x0000FF }
                );
            }

            column += columnsPerDegree;
        }



        flip();
    }
}
