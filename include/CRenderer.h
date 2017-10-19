#ifndef IRENDERER_H
#define IRENDERER_H

#include <set>

namespace odb {

    class CRenderer  : public Knights::IRenderer {
        array<array< int, 40>, 40> mCache;
        array<array< int, 40>, 40> mActors;
        bool mCached = false;
        RayCollision mCurrentScan[ 320 ];
        fixed_point<int32_t, -16> sines[ 360 ];
        fixed_point<int32_t, -16> cossines[ 360 ];
        RayCollision castRay(int d, int offset, const array<array< int, 40>, 40>& map);
        Knights::Vec2i mCamera;
        int mAngle = 0;
        Knights::CommandType mBufferedCommand = '.';
        vector<int> mActorsRendered;
        vector<fixed_point<int32_t, -16>> mCachedDistances;
        vector<int> mCachedAngle;
        bool mNeedsToRedraw = true;
    public:
        void drawMap( Knights::CMap& map, std::shared_ptr<Knights::CActor> current ) override;
        void draw( std::shared_ptr<odb::NativeBitmap> bitmap, int x0, int y0, int w, int h, FixP zValue );
        void bitblt( std::shared_ptr<odb::NativeBitmap> bitmap, int x0, int y0 );
        Knights::CommandType getInput() override;
    private:
        int zBuffer[ 320 ];
        void fill( int x1, int y1, int w, int h, const array<uint8_t,4>& colour );
        void put( int x, int y, const array<uint8_t,4>& colour );
        void flip();
     public:
        CRenderer();
        void render( long ms );
        void handleSystemEvents();
        void sleep( long ms );

        void fillSidebar();

        void fillUnderbar();

        void putRaw(int x, int y, const array<uint8_t, 4>& array);
    };
}
#endif