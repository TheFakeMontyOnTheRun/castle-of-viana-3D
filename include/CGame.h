#ifndef CGAME_H
#define CGAME_H

namespace odb {

    class CGame {
    private:

      CActor mCamera;
        float sines[ 360 ];
        float cossines[ 360 ];
        std::array<std::array< int, 40>, 40 > mMap;
        std::vector<std::shared_ptr<CActor>> mActors;
        RayCollision castRay(int offset);
        void spawnFireball( int x, int y, int angle );
    public:
        explicit CGame();
        void tick( long ms );
        CControlCallback getKeyPressedCallback();
        CControlCallback getKeyReleasedCallback();
        CGameSnapshot getGameSnapshot();
    };
}
#endif
