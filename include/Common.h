//
// Created by monty on 27/08/16.
//

#ifndef BLANKSLATE_COMMON_H
#define BLANKSLATE_COMMON_H
namespace odb {
    enum class ECommand {
        kUp,
        kRight,
        kDown,
        kLeft,
        kFire1,
        kFire2
    };

    class Vec2f {
    public:
      float mX = 0;
      float mY = 0;
    };

    class RayCollision {
    public:
        float mCachedDistance = 0;
        Vec2f mCollisionPoint = {0,0};
        int mHeight = 0;
        int mElement = 0;
    };

    using MapLine = std::array<int, 20>;
    using IntGameMap = std::array<MapLine, 20>;

    using CControlCallback = std::function< void(ECommand) >;


    float Q_rsqrt( float number );
    int wrap360(int i);

    const int xRes = 320;
    const int yRes = 200;

}
#endif //BLANKSLATE_COMMON_H
