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

    SDL_Surface *video;

    CRenderer::CRenderer( CControlCallback keyPressedCallback, CControlCallback keyReleasedCallback ):
    mOnKeyPressedCallback( keyPressedCallback ), mOnKeyReleasedCallback( keyReleasedCallback )
    {
        SDL_Init( SDL_INIT_EVERYTHING );
        video = SDL_SetVideoMode( xRes, yRes, 32, 0 );
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

    void CRenderer::fill( int x1, int y1, int w, int h, std::array<uint8_t ,4> colour ) {
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
}
