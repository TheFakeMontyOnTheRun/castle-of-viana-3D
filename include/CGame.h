#ifndef CGAME_H
#define CGAME_H

namespace odb {

    class CGame {
    private:
      Vec2f mCamera = { 0, 0 };
        float mAngle = 0;
        float sines[ 360 ];
        float cossines[ 360 ];
        std::array<std::array< int, 40>, 40 > mMap;
        RayCollision castRay(int offset);
    public:
        explicit CGame();
        void tick( long ms );
        CControlCallback getKeyPressedCallback();
        CControlCallback getKeyReleasedCallback();
        CGameSnapshot getGameSnapshot();
    };
}
#endif
