#ifndef CGAME_H
#define CGAME_H

namespace odb {

    class CGame {
    private:

      std::shared_ptr<CActor> mCamera;
        float sines[ 360 ];
        float cossines[ 360 ];
        std::array<std::array< int, 40>, 40 > mMap;
        std::vector<std::shared_ptr<CActor>> mActors;
        RayCollision castRay(int offset);
        void spawnFireball( float x, float y, float angle );
    public:
        explicit CGame();
        void tick( long ms );
        CControlCallback getKeyPressedCallback();
        CControlCallback getKeyReleasedCallback();
        CGameSnapshot getGameSnapshot();
    };
}
#endif
