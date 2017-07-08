#ifndef IRENDERER_H
#define IRENDERER_H

#include "CGameSnapshot.h"
#include "NativeBitmap.h"

namespace odb {



    class CRenderer {
    private:
        CControlCallback mOnKeyPressedCallback;
        CControlCallback mOnKeyReleasedCallback;
        CGameSnapshot mGameSnapshot;
        void fill( int x1, int y1, int w, int h, std::array<uint8_t,4> colour );
        void draw( std::shared_ptr<odb::NativeBitmap> bitmap, int x0, int y0, int w, int h );
        void flip();
     public:
        CRenderer( CControlCallback keyPressedCallback, CControlCallback keyReleasedCallback );
        void render( long ms );
        void handleSystemEvents();
        void sleep( long ms );
        void setSnapshot( const CGameSnapshot& snapshot );
    };
}
#endif