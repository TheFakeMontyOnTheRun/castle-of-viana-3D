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

    void CRenderer::render( long ms ) {
        SDL_Rect rect;

        rect = { 0, 0, xRes, yRes };
        SDL_FillRect( video, &rect, SDL_MapRGB( video->format, 96, 96, 96 ) );

        rect = { 0, yRes / 2, xRes, yRes / 2};
        SDL_FillRect( video, &rect, SDL_MapRGB( video->format, 192, 192, 192 ) );


        constexpr auto columnsPerDegree = xRes / 90;
        auto column = 0;

        for (int d = -45; d < 45; ++d) {

            float ray = mGameSnapshot.mCurrentScan[ d + 45 ].mCachedDistance;
            int distance = (yRes / ray);
            float sin_a = sin( wrap360( mGameSnapshot.mAngle + d) * ( 3.14159f / 180.0f) );
            float cos_a = cos( wrap360( mGameSnapshot.mAngle + d) * ( 3.14159f / 180.0f) );
            float hueX =  mGameSnapshot.mCamera.mX + ( ray * ( sin_a ) );
            float hueZ = mGameSnapshot.mCamera.mY + ( ray * ( cos_a ) );

            int cellX = hueX;
            int cellZ = hueZ;

            int dx = ( hueX - cellX ) * 32;
            int dz = ( hueZ - cellZ ) * 32;

            int columnHeight = distance;

            for ( int y = 0; y < columnHeight; ++y ) {

                int v = ( texture->getHeight() * y) / columnHeight;
                int ux = (texture->getWidth() * dx) / 40;
                int uz = (texture->getWidth() * dz) / 40;
                int pixel = texture->getPixelData()[ ( texture->getWidth() * v ) + ((ux + uz ) % texture->getWidth()) ];

                rect = SDL_Rect{static_cast<Sint16 >(column),
                                static_cast<Sint16 >(yRes / 2 - (distance / 2) + y ),
                                static_cast<Uint16 >(columnsPerDegree),
                                static_cast<Uint16 >(1)};

                SDL_FillRect(video, &rect, SDL_MapRGB(video->format, pixel & 0xFF000000 >> 8, pixel & 0x00FF0000 >> 4, pixel & 0x0000FF ) );
            }

            column += columnsPerDegree;
        }



        SDL_Flip(video);
    }
}
