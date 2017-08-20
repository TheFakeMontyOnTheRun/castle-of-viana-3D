#ifndef IRENDERER_H
#define IRENDERER_H

namespace odb {

    class CRenderer  : public Knights::IRenderer {

        RayCollision mCurrentScan[ 90 ];
        fixed_point<int32_t, -16> sines[ 360 ];
        fixed_point<int32_t, -16> cossines[ 360 ];
        RayCollision castRay(uint32_t offset, const Knights::CMap& map);
        Knights::Vec2i mCamera;
        int mAngle = 0;
        Knights::CommandType mBufferedCommand = '.';
    public:
        void drawMap( Knights::CMap& map, std::shared_ptr<Knights::CActor> current ) override;
        Knights::CommandType getInput() override;
    private:
        int zBuffer[ 320 ];
        void fill( int x1, int y1, int w, int h, std::array<uint8_t,4> colour );
        void flip();
     public:
        CRenderer();
        void render( long ms );
        void handleSystemEvents();
        void sleep( long ms );
    };
}
#endif