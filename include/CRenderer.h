#ifndef IRENDERER_H
#define IRENDERER_H

#include <Vec2i.h>
#include <CActor.h>
#include "NativeBitmap.h"

namespace odb {

    class CRenderer  : public Knights::IRenderer {

        RayCollision mCurrentScan[ 90 ];
        float sines[ 360 ];
        float cossines[ 360 ];
        RayCollision castRay(float offset, const Knights::CMap& map);
        Knights::Vec2i mCamera;
        int mAngle = 0;
        Knights::CommandType mBufferedCommand = '.';
    public:
        void drawMap( Knights::CMap& map, std::shared_ptr<Knights::CActor> current ) override;
        Knights::CommandType getInput() override;
    private:
        int zBuffer[ 320 ];
        void fill( int x1, int y1, int w, int h, std::array<uint8_t,4> colour );
        void draw( std::shared_ptr<odb::NativeBitmap> bitmap, int x0, int y0, int w, int h, int zValue );
        void flip();
     public:
        CRenderer();
        void render( long ms );
        void handleSystemEvents();
        void sleep( long ms );
    };
}
#endif