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

    enum class EActorType {
        kEnemy,
        kFireball
    };

    class Vec2f {
    public:
      float mX = 0;
      float mY = 0;
    };

    class RayCollision {
    public:
        float mSquaredDistance = 0;
        Vec2f mCollisionPoint = {0,0};
        int mHeight = 0;
        int mElement = 0;
    };

    class CActor {
    public:
        Vec2f mPosition = { 0, 0 };
        Vec2f mSpeed = { 0, 0 };
        float mAngularSpeed = 0;
        float mAngle = 0;
        EActorType mType = EActorType::kEnemy;
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
