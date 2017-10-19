#include <string>
#include <utility>
#include <functional>
#include <memory>
#include <map>
#include <sg14/fixed_point>
#include <EASTL/vector.h>
#include <EASTL/array.h>

using eastl::vector;
using eastl::array;

using sg14::fixed_point;

#include "RaycastCommon.h"
#include "Vec2i.h"
#include "IMapElement.h"
#include "CTeam.h"
#include "CItem.h"
#include "CActor.h"
#include "CGameDelegate.h"
#include "CMap.h"
#include "IRenderer.h"
#include "IFileLoaderDelegate.h"
#include "CGame.h"
#include "NativeBitmap.h"
#include "CRenderer.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <cmath>
#include "NativeBitmap.h"
#include "LoadPNG.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

namespace odb {

    SDL_Surface *video;

#ifdef __EMSCRIPTEN__
    void enterFullScreenMode() {
    EmscriptenFullscreenStrategy s;
    memset(&s, 0, sizeof(s));
    s.scaleMode = EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT;
    s.canvasResolutionScaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE;
    s.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
    emscripten_enter_soft_fullscreen(0, &s);
}
#endif


    CRenderer::CRenderer() {
        SDL_Init( SDL_INIT_EVERYTHING );
        video = SDL_SetVideoMode( 320, 200, 32, 0 );

#ifdef __EMSCRIPTEN__
        enterFullScreenMode();
#endif

        for ( int c = 0; c < 360; ++c ) {
            auto sin_a = fixed_point<int32_t , -16>{(std::sin((c * 3.14159f) / 180.0f)) / 16.0f};
            auto cos_a = fixed_point<int32_t , -16>{(std::cos((c * 3.14159f) / 180.0f)) / 16.0f};
            sines[ c ] = sin_a;
            cossines[ c ] = cos_a;
        }

    }

    void CRenderer::sleep( long ms ) {
#ifndef __EMSCRIPTEN__
        SDL_Delay(33);
#endif
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
		default:
		  break;
                }
            }

            if ( event.type == SDL_KEYDOWN ) {
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        mBufferedCommand = Knights::kUseCurrentItemInInventoryCommand;
                        mNeedsToRedraw = true;
                        mCached = false;
                        break;

                    case SDLK_LEFT:
                        mBufferedCommand = Knights::kTurnPlayerLeftCommand;
                        mNeedsToRedraw = true;
                        mCached = false;
                        break;
                    case SDLK_RIGHT:
                        mBufferedCommand = Knights::kTurnPlayerRightCommand;
                        mNeedsToRedraw = true;
                        mCached = false;
                        break;
                    case SDLK_UP:
                        mBufferedCommand = Knights::kMovePlayerForwardCommand;
                        mNeedsToRedraw = true;
                        mCached = false;
                        break;
                    case SDLK_DOWN:
                        mBufferedCommand = Knights::kMovePlayerBackwardCommand;
                        mNeedsToRedraw = true;
                        mCached = false;
                        break;

                    default:
                        break;
                }
            }

        }
    }

    void CRenderer::fill( int x1, int y1, int w, int h, const array<uint8_t ,4>& colour ) {
        SDL_Rect rect;
        rect.x = x1;
        rect.y = y1;
        rect.w = w;
        rect.h = h;

        SDL_FillRect(video, &rect, SDL_MapRGB(video->format, colour[ 1 ], colour[ 2 ], colour[ 3 ] ) );
    }

    void CRenderer::put( int x, int y, const array<uint8_t ,4>& colour ) {
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = 1;
        rect.h = 1;

        SDL_FillRect(video, &rect, SDL_MapRGB(video->format, colour[ 1 ], colour[ 2 ], colour[ 3 ] ) );
    }

    void CRenderer::putRaw( int x, int y, const array<uint8_t ,4>& colour ) {
        put( x, y, colour );
    }

    void CRenderer::flip() {
        SDL_Flip(video);
    }
}
